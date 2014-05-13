#include <vector>
#include <cassert>

#include <opengl.h>
#include <al/al.h>
#include <al/alc.h>

#include <SDL2/SDL.h>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

#include "Timer.h"
#include "profiler.h"
#include "ResourceHelpers.h"

//Simple ring buffer for AVPacket
//Relies on overflow behaviour
#define PACKET_BUFFER_SIZE 64
struct AVPacketRingBuffer
{
    unsigned int head;
    unsigned int tail;
    AVPacket     buf[PACKET_BUFFER_SIZE];
};

void resetAVPackets(AVPacketRingBuffer* rbuf)
{
    rbuf->head = rbuf->tail = 0;
}

unsigned int usedAVPackets(AVPacketRingBuffer* rbuf)
{
    return rbuf->head-rbuf->tail;
}

AVPacket* allocAVPacket(AVPacketRingBuffer* rbuf)
{
    return (usedAVPackets(rbuf) < PACKET_BUFFER_SIZE) ? rbuf->buf + (rbuf->head++%PACKET_BUFFER_SIZE) : 0;
}

AVPacket* frontAVPacket(AVPacketRingBuffer* rbuf)
{
    return (usedAVPackets(rbuf) != 0) ? rbuf->buf+(rbuf->tail % PACKET_BUFFER_SIZE) : 0;
}

void popAVPacket(AVPacketRingBuffer* rbuf)
{
    if (usedAVPackets(rbuf) != 0) rbuf->tail++;
}

struct media_player_data_t
{
    AVFormatContext* formatContext;
    AVCodecContext*  videoCodecContext;
    AVCodecContext*  audioCodecContext;
    unsigned int     videoStream;
    unsigned int     audioStream;
    AVFrame*         pFrame; 
    int              mAudioFormat;
    int              numBytes;
    int              width;
    int              height;
    PixelFormat      srcFormat;
    AVRational       frameDuration;
    int              playback;
    int              streamEnd;

    AVPacketRingBuffer aPackets;
    AVPacketRingBuffer vPackets;

    __int64 baseTime, timeShift, timeOfNextFrame, frameTime;
    __int64 aBufferSize;

    GLuint texY[2];
    GLuint texU[2];
    GLuint texV[2];

    ALuint		audioSource;
    ALuint		audioBuffers[2];

    int bufSize;

    char __declspec(align(16)) audioBuf[AVCODEC_MAX_AUDIO_FRAME_SIZE]; // make it part of thread scratch buffer

    int subTasks;
    
    ALuint bufferToUpdate;

    uint32_t eventID;
    bool     taskStarted;
};

typedef struct media_player_data_t* media_player_t;

static int extAudioFormatsPresent;

const char srcYUV2RGB[] = 
    "layout(binding = 0) uniform sampler2D samY;                                                    \n"
    "layout(binding = 1) uniform sampler2D samU;                                                    \n"
    "layout(binding = 2) uniform sampler2D samV;                                                    \n"
    "                                                                                               \n"
    "void main()                                                                                    \n"
    "{                                                                                              \n"
    "    float y   = texture2D(samY, gl_TexCoord[0].xy).r;                                          \n"
    "    float u   = texture2D(samU, gl_TexCoord[0].xy).r - 0.5;                                    \n"
    "    float v   = texture2D(samV, gl_TexCoord[0].xy).r - 0.5;                                    \n"
    "                                                                                               \n"
    "    vec3 rgb = vec3(y) + u*vec3(0.0, -0.344, 1.770) + v*vec3(1.403, -0.714, 0.0);              \n"
    "                                                                                               \n"
    "    gl_FragColor = vec4(rgb, 1.0)*gl_Color;                                                    \n"
    "}                                                                                              \n";

GLuint progYUV2RGB;

//TODO: implement pool, maybe fixed at first!!!!
media_player_data_t hack;

static void closeAudioStream(media_player_t player)
{
    if (player->audioCodecContext)
    {
        avcodec_close(player->audioCodecContext);

        alSourceStop(player->audioSource);
        alDeleteBuffers(2, player->audioBuffers);
        alDeleteSources(1, &player->audioSource);

        player->audioCodecContext = 0;
    }
}

static int openAudioStream(media_player_t player, AVCodecContext* audioContext)
{
    AVCodec* pAudioCodec;
    int      channels;

    closeAudioStream(player);

    //FFMPEG always decodes to 16 PCM
    channels = audioContext->channels;
    if(channels == 1) player->mAudioFormat = AL_FORMAT_MONO16;
    else if(channels == 2) player->mAudioFormat = AL_FORMAT_STEREO16;
    else if(extAudioFormatsPresent && channels == 4) player->mAudioFormat = alGetEnumValue("AL_FORMAT_QUAD16");
    else if(extAudioFormatsPresent && channels == 6) player->mAudioFormat = alGetEnumValue("AL_FORMAT_51CHN16");
    else
    {
        //TODO: encode in AL_FORMAT_STEREO16
        return 0;
    }

    pAudioCodec=avcodec_find_decoder(audioContext->codec_id);
    if(pAudioCodec==NULL || avcodec_open(audioContext, pAudioCodec)<0)
    {
        return 0;
    }

    player->audioCodecContext = audioContext;

    alGenSources(1, &player->audioSource);
    alGenBuffers(2, player->audioBuffers);

    //TODO: HARDCODE!!!!!!!
    player->aBufferSize  = player->audioCodecContext->sample_rate / 25 * 3/*frames to buffer*/ * 2/*num channels*/ * 2/*bits per channel*/;

    return 1;
}

static void closeVideoStream(media_player_t player)
{
    if (player->videoCodecContext)
    {
        av_free(player->pFrame);

        avcodec_close(player->videoCodecContext);

        glDeleteTextures(2, player->texY );
        glDeleteTextures(2, player->texU);
        glDeleteTextures(2, player->texV);

        player->videoCodecContext = 0;
    }
}

static void initTexture(GLuint tex, GLuint width, GLuint height)
{
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE8, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
}

static int openVideoStream(media_player_t player, AVCodecContext* videoContext)
{
    closeVideoStream(player);

    AVCodec* pVideoCodec=avcodec_find_decoder(videoContext->codec_id);
    if(pVideoCodec==NULL || avcodec_open(videoContext, pVideoCodec)<0)
        return 0;

    // Hack to correct wrong frame rates that seem to be generated by some codecs
    // if(pVideoCodecCtx->frame_rate>1000 && pVideoCodecCtx->frame_rate_base==1)
    //    pVideoCodecCtx->frame_rate_base=1000;

    player->videoCodecContext = videoContext;
    player->pFrame            = avcodec_alloc_frame();
    player->numBytes          = avpicture_get_size(PIX_FMT_YUV420P, videoContext->width, videoContext->height);

    resetAVPackets(&player->aPackets);
    resetAVPackets(&player->vPackets);

    player->width     = videoContext->width;
    player->height    = videoContext->height;
    player->srcFormat = videoContext->pix_fmt;
    assert(player->srcFormat == PIX_FMT_YUV420P);

    glGenTextures(2, player->texY);
    glGenTextures(2, player->texU);
    glGenTextures(2, player->texV);

    initTexture(player->texY[0], player->width,   player->height  );
    initTexture(player->texY[1], player->width,   player->height  );
    initTexture(player->texU[0], player->width/2, player->height/2);
    initTexture(player->texU[1], player->width/2, player->height/2);
    initTexture(player->texV[0], player->width/2, player->height/2);
    initTexture(player->texV[1], player->width/2, player->height/2);

    return 1;
}

static void streamMediaData(media_player_t player)
{
    PROFILER_CPU_TIMESLICE("streamMediaData");

    AVPacket packet;

    if (!player->streamEnd && av_read_frame(player->formatContext, &packet)>=0)
    {
        if (packet.stream_index==player->audioStream)
        {
            *allocAVPacket(&player->aPackets) = packet;
        }
        else if (packet.stream_index==player->videoStream)
        {
            *allocAVPacket(&player->vPackets) = packet;
        }
    }
    else if (!player->streamEnd)
    {
        player->streamEnd = true;
        packet.data = 0;
        packet.size = 0;
        *allocAVPacket(&player->vPackets) = packet;
    }
}

static void uploadAudioData(media_player_t player, ALuint buffer)
{
    PROFILER_CPU_TIMESLICE("uploadAudioData");

    alBufferData(buffer, player->mAudioFormat, player->audioBuf, player->bufSize, player->audioCodecContext->sample_rate);
    alSourceQueueBuffers(player->audioSource, 1, &buffer);
}

static void decodeAudio(media_player_t player)
{
    PROFILER_CPU_TIMESLICE("decodeAudio");

    int audioDataAva;
    
    audioDataAva    = 1;
    player->bufSize = 0;

    while((player->bufSize < player->aBufferSize) && audioDataAva)
    {
        AVPacket* pkt = frontAVPacket(&player->aPackets);
        if (pkt)
        {
            uint8_t*	inData     = pkt->data;
            int			inSize     = pkt->size;
            int         bufSizeAva = AVCODEC_MAX_AUDIO_FRAME_SIZE;

            while(inSize > 0)
            {
                int processed = avcodec_decode_audio2(player->audioCodecContext, (int16_t*)(player->audioBuf+player->bufSize), &bufSizeAva, inData, inSize);

                if(processed < 0)
                {
                    break;
                }

                inData     += processed;
                inSize     -= processed;

                assert(player->bufSize+bufSizeAva<AVCODEC_MAX_AUDIO_FRAME_SIZE);
                player->bufSize += bufSizeAva;
            }

            av_free_packet(pkt);
            popAVPacket(&player->aPackets);
        }
        else
            streamMediaData(player);

        audioDataAva = !player->streamEnd || usedAVPackets(&player->aPackets)!=0;
    }
}

static void uploadVideoData(media_player_t player, GLuint texY, GLuint texU, GLuint texV)
{
    PROFILER_CPU_TIMESLICE("uploadVideoData");

    glBindTexture(GL_TEXTURE_2D, texY);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, player->pFrame->linesize[0]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, player->width, player->height, GL_LUMINANCE, GL_UNSIGNED_BYTE, player->pFrame->data[0]);

    glBindTexture(GL_TEXTURE_2D, texU);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, player->pFrame->linesize[1]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, player->width/2, player->height/2, GL_LUMINANCE, GL_UNSIGNED_BYTE, player->pFrame->data[1]);

    glBindTexture(GL_TEXTURE_2D, texV);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, player->pFrame->linesize[2]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, player->width/2, player->height/2, GL_LUMINANCE, GL_UNSIGNED_BYTE, player->pFrame->data[2]);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

    player->timeOfNextFrame = player->frameTime;

    GLenum err = glGetError();
    assert(err == GL_NO_ERROR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

static void decodeVideo(media_player_t player)
{
    PROFILER_CPU_TIMESLICE("decodeVideo");

    int frameDone    = 0;
    int videoDataAva = 1;

    while(videoDataAva)
    {
        AVPacket* pkt = frontAVPacket(&player->vPackets);
        if (pkt)
        {
            avcodec_decode_video(player->videoCodecContext, player->pFrame, &frameDone, pkt->data, pkt->size);

            av_free_packet(pkt);
            popAVPacket(&player->vPackets);
        }
        else
            streamMediaData(player);

        if(frameDone)
        {
            player->frameTime = player->frameDuration.num*pkt->dts*1000000/player->frameDuration.den;
            break;
        }

        videoDataAva = !player->streamEnd || usedAVPackets(&player->vPackets)!=0;
    }
}

#ifdef _DEBUG
char errBuf[2048];
#endif

void mediaInit()
{
    // Register all formats and codecs
    av_register_all();

    progYUV2RGB = resources::createProgram(GL_FRAGMENT_SHADER, srcYUV2RGB);

    extAudioFormatsPresent = alIsExtensionPresent("AL_EXT_MCFORMATS");
}

void mediaShutdown()
{
    glDeleteProgram(progYUV2RGB);
}

media_player_t mediaCreatePlayer(const char* source)
{
    media_player_t player = (media_player_t)malloc(sizeof(media_player_data_t));

    memset(player, 0, sizeof(media_player_data_t));

    if (av_open_input_file(&player->formatContext, source, NULL, 0, NULL)!=0)
        return 0;

    if (av_find_stream_info(player->formatContext)<0)
        return 0 ;

    dump_format(player->formatContext, 0, source, false); // Dump information about file onto standard error

    unsigned int  numStreams = player->formatContext->nb_streams;
    AVStream**    streams    = player->formatContext->streams;

    player->audioStream = numStreams;
    for(unsigned int i=0; i<numStreams; i++)
    {
        AVCodecContext* audioContext = streams[i]->codec;
        if(audioContext->codec_type==CODEC_TYPE_AUDIO && openAudioStream(player, audioContext))
        {
            player->audioStream = i;
            break;
        }
    }

    player->videoStream = numStreams;
    for(unsigned int i=0; i<numStreams; i++)
    {
        AVCodecContext* videoContext = streams[i]->codec;
        if(streams[i]->codec->codec_type==CODEC_TYPE_VIDEO && openVideoStream(player, videoContext))
        {
            player->videoStream   = i;
            player->frameDuration = streams[i]->time_base;
            break;
        }
    }

    player->taskStarted = false;
    player->eventID     = mt::INVALID_HANDLE;

    return player;
}

void mediaDestroyPlayer(media_player_t player)
{
    if (player->taskStarted)
    {
        mt::syncAndReleaseEvent(player->eventID);
    }

    closeAudioStream(player);
    closeVideoStream(player);

    av_close_input_file(player->formatContext);
    free(player);
}

enum 
{
    MEDIA_DECODE_VIDEO = 1,
    MEDIA_DECODE_AUDIO = 2,
};

static void doDecode(media_player_t player)
{
    PROFILER_CPU_TIMESLICE("mediaPlayerUpdateTask");

    if (player->subTasks&MEDIA_DECODE_VIDEO)
    {
        decodeVideo(player);
    }
    if (player->subTasks&MEDIA_DECODE_AUDIO)
    {
        decodeAudio(player);
    }
}

static void decodeTask(void* arg)
{
    media_player_t player = (media_player_t)arg;

    doDecode(player);
}

void mediaStartPlayback(media_player_t player)
{
    player->timeOfNextFrame = 0;
    player->streamEnd       = FALSE;

    player->subTasks = MEDIA_DECODE_VIDEO|MEDIA_DECODE_AUDIO;
    doDecode(player);
    uploadAudioData(player, player->audioBuffers[0]);
    uploadVideoData(player, player->texY[0], player->texU[0], player->texV[0]);
    player->timeShift = player->frameTime; //in some videos first dts differs from 0

    player->subTasks = MEDIA_DECODE_VIDEO|MEDIA_DECODE_AUDIO;
    doDecode(player);
    uploadAudioData(player, player->audioBuffers[1]);
    uploadVideoData(player, player->texY[1], player->texU[1], player->texV[1]);

    player->playback  = TRUE;
    player->baseTime  = timerAbsoluteTime();

    alSourcePlay(player->audioSource);
}

void mediaPlayerUpdate(media_player_t player)
{
    PROFILER_CPU_TIMESLICE("mediaPlayerUpdate");

    if (!player->playback)
        return;

    __int64 currentTime = timerAbsoluteTime()-player->baseTime;

    ALuint buffer;

    player->playback = !player->streamEnd || usedAVPackets(&player->aPackets)!=0 || usedAVPackets(&player->vPackets)!=0;

    if (player->taskStarted)
    {
        mt::syncAndReleaseEvent(player->eventID);

        if (player->subTasks&MEDIA_DECODE_VIDEO)
        {
            uploadVideoData(player, player->texY[1], player->texU[1], player->texV[1]);
        }
        if (player->subTasks&MEDIA_DECODE_AUDIO)
        {
            uploadAudioData(player, player->bufferToUpdate);
        }
    }

    player->taskStarted = false;
    player->subTasks    = 0;

    {
        ALint  count;
        alGetSourcei(player->audioSource, AL_BUFFERS_PROCESSED, &count);
        if (count>0)
        {
            alSourceUnqueueBuffers(player->audioSource, 1, &buffer);
            player->subTasks |= MEDIA_DECODE_AUDIO;
            player->bufferToUpdate = buffer;
        }

        ALint state;
        alGetSourcei(player->audioSource, AL_SOURCE_STATE, &state);
        if (state != AL_PLAYING)
        {
            alSourcePlay(player->audioSource);
        }
    }

    int64_t time = currentTime+player->timeShift;
    if (time>=player->timeOfNextFrame)
    {
        std::swap(player->texY[0], player->texY[1]);
        std::swap(player->texU[0], player->texU[1]);
        std::swap(player->texV[0], player->texV[1]);

        player->subTasks |= MEDIA_DECODE_VIDEO;
    }

    if (player->subTasks)
    {
        player->taskStarted = true;
        mt::addAsyncTask(decodeTask, player, &player->eventID);
    }
}

void mediaPlayerPrepareRender(media_player_t player)
{
    glUseProgram(progYUV2RGB);

    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, player->texY[0]);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, player->texU[0]);
    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, player->texV[0]);

    glActiveTexture(GL_TEXTURE0);
#ifdef _DEBUG
    GLenum err = glGetError();
    assert(err==GL_NO_ERROR);
#endif
}

void mediaPlayerRender(media_player_t player)
{
    glUseProgram(progYUV2RGB);

    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, player->texY[0]);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, player->texU[0]);
    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, player->texV[0]);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
    glEnd();

    glUseProgram(0);

#ifdef _DEBUG
    GLenum err = glGetError();
    assert(err==GL_NO_ERROR);
#endif
}