#include <vector>
#include <cassert>

#include <opengl.h>
#include <al/al.h>
#include <al/alc.h>

extern "C"
{
#include <avformat.h>
#include <avcodec.h>
#include <swscale.h>
}

uint64_t timerAbsoluteTime();

#define NUM_BUFFERS 3

static const int aFramesCache = 2;
static const int vFramesCache = 1;

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
    return (usedAVPackets(rbuf) < PACKET_BUFFER_SIZE) ? (rbuf->buf + rbuf->head++%PACKET_BUFFER_SIZE) : 0;
}

AVPacket* frontAVPacket(AVPacketRingBuffer* rbuf)
{
    return (usedAVPackets(rbuf) != 0) ? rbuf->buf+(rbuf->tail % PACKET_BUFFER_SIZE) : 0;
}

void popAVPacket(AVPacketRingBuffer* rbuf)
{
    if (usedAVPackets != 0) rbuf->tail++;
}

struct media_player_data_t
{

    AVFormatContext* pFormatCtx;
    AVCodecContext*  pVideoCodecCtx;
    AVCodecContext*  pAudioCodecCtx;
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

    __int64 baseTime, timeShift, timeOfNextFrame;

    __int64 FPS;
    __int64 vFrameTime;
    __int64 aFrameSamples, aSamplesDecoded, aBufferSamples;
    __int64 aFramesAva, vFramesAva;

    GLuint texY[2];
    GLuint texU[2];
    GLuint texV[2];
    GLuint pbo;

    ALuint		audioSource;
    ALuint		audioBuffers[NUM_BUFFERS];
    int			availableAudioBuffers;

    char __declspec(align(16)) audioBuf[AVCODEC_MAX_AUDIO_FRAME_SIZE];
    char __declspec(align(16)) audioBuf2[AVCODEC_MAX_AUDIO_FRAME_SIZE];
    int                        bufSize;
};

typedef struct media_player_data_t* media_player_t;

static int                        extAudioFormatsPresent;

const char srcYUV2RGB[] = 
    "uniform sampler2D samY;                                                                        \n"
    "uniform sampler2D samU;                                                                        \n"
    "uniform sampler2D samV;                                                                        \n"
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
    if (player->pAudioCodecCtx)
    {
        avcodec_close(player->pAudioCodecCtx);

        ALint enqueuedBuffers;
        alSourceStop(player->audioSource);
        alGetSourcei(player->audioSource, AL_BUFFERS_QUEUED, &enqueuedBuffers);
        assert(player->availableAudioBuffers+enqueuedBuffers == NUM_BUFFERS);
        alSourceUnqueueBuffers(player->audioSource, enqueuedBuffers, player->audioBuffers+player->availableAudioBuffers);
        alDeleteSources(1, &player->audioSource);
        alDeleteBuffers(NUM_BUFFERS, player->audioBuffers);

        player->pAudioCodecCtx = 0;
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

    player->pAudioCodecCtx = audioContext;

    alGenSources(1, &player->audioSource);
    alGenBuffers(NUM_BUFFERS, player->audioBuffers);

    //TODO: HARDCODE!!!!!!!
    player->aBufferSamples  = player->pAudioCodecCtx->sample_rate / 25 * 2 * 2 * 2;
    player->aSamplesDecoded = 0;

    player->availableAudioBuffers = NUM_BUFFERS;

    return 1;
}

static void closeVideoStream(media_player_t player)
{
    if (player->pVideoCodecCtx)
    {
        av_free(player->pFrame);

        avcodec_close(player->pVideoCodecCtx);

        glDeleteTextures(2, player->texY );
        glDeleteTextures(2, player->texU);
        glDeleteTextures(2, player->texV);

        glDeleteBuffers(1, &player->pbo);
        player->pVideoCodecCtx = 0;
        player->bufSize = 0;
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

    player->pVideoCodecCtx = videoContext;
    player->pFrame         = avcodec_alloc_frame();
    player->numBytes       = avpicture_get_size(PIX_FMT_YUV420P, videoContext->width, videoContext->height);

    resetAVPackets(&player->aPackets);
    resetAVPackets(&player->vPackets);

    player->vFramesAva = 0;

    player->width     = videoContext->width;
    player->height    = videoContext->height;
    player->srcFormat = videoContext->pix_fmt;
    assert(player->srcFormat == PIX_FMT_YUV420P);

    glGenBuffers(1, &player->pbo);

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
    AVPacket packet;

    if (!player->streamEnd && av_read_frame(player->pFormatCtx, &packet)>=0)
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

static void decodeAudio(media_player_t player, __int64 time)
{
    __int64 aSamples = (time<0?0:time) / 1000 * player->pAudioCodecCtx->sample_rate / 1000;

    int audioDataAva = 1;
    while((aSamples+player->aBufferSamples)>player->aSamplesDecoded && audioDataAva)
    {
        AVPacket* pkt = frontAVPacket(&player->aPackets);
        if (pkt)
        {
            uint8_t*	inData     = pkt->data;
            int			inSize     = pkt->size;
            int         bufSizeAva = AVCODEC_MAX_AUDIO_FRAME_SIZE;

            while(inSize > 0)
            {
                int processed = avcodec_decode_audio2(player->pAudioCodecCtx, (int16_t*)player->audioBuf, &bufSizeAva, inData, inSize);

                if(processed < 0)
                {
                    break;
                }

                inData     += processed;
                inSize     -= processed;

                if (processed>0)
                {
                    assert(player->bufSize+bufSizeAva<AVCODEC_MAX_AUDIO_FRAME_SIZE);
                    memcpy(player->audioBuf2+player->bufSize, player->audioBuf, bufSizeAva);
                    player->bufSize += bufSizeAva;
                    player->aSamplesDecoded+=bufSizeAva/2/2;
                }
            }

            av_free_packet(pkt);
            popAVPacket(&player->aPackets);
        }
        else
            streamMediaData(player);

        audioDataAva = !player->streamEnd || usedAVPackets(&player->aPackets)!=0;
    }

    ALint  count;
    alGetSourcei(player->audioSource, AL_BUFFERS_PROCESSED, &count);
    if (count>0)
    {
        ALuint buffer;
        alSourceUnqueueBuffers(player->audioSource, 1, &buffer);
        assert(player->availableAudioBuffers<NUM_BUFFERS);
        player->audioBuffers[player->availableAudioBuffers++] = buffer;
    }

    if (player->bufSize>0 && player->availableAudioBuffers>0)
    {
        ALuint buffer = player->audioBuffers[--player->availableAudioBuffers];
        alBufferData(buffer, player->mAudioFormat, player->audioBuf2, player->bufSize, player->pAudioCodecCtx->sample_rate);
        alSourceQueueBuffers(player->audioSource, 1, &buffer);
        player->bufSize = 0;
    }

    ALint state;
    alGetSourcei(player->audioSource, AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING)
    {
        alSourcePlay(player->audioSource);
    }
}

static void decodeFrame(media_player_t player, __int64 time)
{
    int frameDone, videoDataAva;

    if (time<player->timeOfNextFrame) return;

    frameDone    = 0;
    videoDataAva = 1;
    while(!frameDone && videoDataAva)
    {
        player->playback = !player->streamEnd || usedAVPackets(&player->aPackets)!=0 || usedAVPackets(&player->vPackets)!=0;

        AVPacket* pkt = frontAVPacket(&player->vPackets);
        if (pkt)
        {
            avcodec_decode_video(player->pVideoCodecCtx, player->pFrame, &frameDone, pkt->data, pkt->size);

            av_free_packet(pkt);
            popAVPacket(&player->vPackets);
        }
        else
            streamMediaData(player);

        if(frameDone)
        {
            player->timeOfNextFrame = player->frameDuration.num*pkt->dts*1000000/player->frameDuration.den;

            size_t sizeY;
            size_t sizeU;
            size_t sizeV;
            size_t offsetU;
            size_t offsetV;
            size_t bufferSize;

            sizeY = player->pFrame->linesize[0]*player->height;
            sizeU = player->pFrame->linesize[1]*player->height/2;
            sizeV = player->pFrame->linesize[2]*player->height/2;

            offsetU    = sizeY;
            offsetV    = offsetU+sizeU;
            bufferSize = offsetV+sizeV;

            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, player->pbo);
            glBufferData(GL_PIXEL_UNPACK_BUFFER, bufferSize, 0, GL_STREAM_DRAW);

            uint8_t* ptr = (uint8_t*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);

            memcpy(ptr,         player->pFrame->data[0], sizeY);
            memcpy(ptr+offsetU, player->pFrame->data[1], sizeU);
            memcpy(ptr+offsetV, player->pFrame->data[2], sizeV);

            glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

            glBindTexture(GL_TEXTURE_2D, player->texY[1]);
            glPixelStorei(GL_UNPACK_ROW_LENGTH, player->pFrame->linesize[0]);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, player->width, player->height, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);

            glBindTexture(GL_TEXTURE_2D, player->texU[1]);
            glPixelStorei(GL_UNPACK_ROW_LENGTH, player->pFrame->linesize[1]);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, player->width/2, player->height/2, GL_LUMINANCE, GL_UNSIGNED_BYTE, (void*)offsetU);

            glBindTexture(GL_TEXTURE_2D, player->texV[1]);
            glPixelStorei(GL_UNPACK_ROW_LENGTH, player->pFrame->linesize[2]);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, player->width/2, player->height/2, GL_LUMINANCE, GL_UNSIGNED_BYTE, (void*)offsetV);

            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

            GLenum err = glGetError();
            assert(err == GL_NO_ERROR);

            glBindTexture(GL_TEXTURE_2D, 0);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

            std::swap(player->texY[0], player->texY[1]);
            std::swap(player->texU[0], player->texU[1]);
            std::swap(player->texV[0], player->texV[1]);
        }
        videoDataAva = !player->streamEnd || usedAVPackets(&player->vPackets)!=0;
    }
}

#ifdef _DEBUG
char errBuf[2048];
#endif

void mediaInit()
{
    const char* src;
    GLsizei     len, res;
    GLuint      fs;
    GLuint      uni;

    // Register all formats and codecs
    av_register_all();

    fs  = glCreateShader(GL_FRAGMENT_SHADER);
    progYUV2RGB = glCreateProgram();

    src = srcYUV2RGB;
    len = sizeof(srcYUV2RGB);

    glShaderSource(fs, 1, (const GLchar**)&src, &len);
    glCompileShader(fs);

#ifdef _DEBUG
    glGetShaderInfoLog(fs, sizeof(errBuf), &len, errBuf); 
    glGetShaderiv(fs, GL_COMPILE_STATUS, &res);
    assert(res);
#endif

    glAttachShader(progYUV2RGB, fs);
    glLinkProgram(progYUV2RGB);

#ifdef _DEBUG
    glGetProgramInfoLog(progYUV2RGB, sizeof(errBuf), &len, errBuf); 
    glGetProgramiv(progYUV2RGB, GL_LINK_STATUS, &res);
    assert(res);
#endif

    glDeleteShader(fs);

    glUseProgram(progYUV2RGB);
    uni = glGetUniformLocation(progYUV2RGB, "samY"); glUniform1i(uni, 0);
    uni = glGetUniformLocation(progYUV2RGB, "samU"); glUniform1i(uni, 1);
    uni = glGetUniformLocation(progYUV2RGB, "samV"); glUniform1i(uni, 2);
    glUseProgram(0);

#ifdef _DEBUG
    GLenum err = glGetError();
    assert(err==GL_NO_ERROR);
#endif

    extAudioFormatsPresent = alIsExtensionPresent("AL_EXT_MCFORMATS");
}

void mediaShutdown()
{
    glDeleteProgram(progYUV2RGB);
}

media_player_t mediaCreatePlayer(const char* source)
{
    media_player_t player = &hack;

    if (av_open_input_file(&player->pFormatCtx, source, NULL, 0, NULL)!=0)
        return 0;

    if (av_find_stream_info(player->pFormatCtx)<0)
        return 0 ;

    dump_format(player->pFormatCtx, 0, source, false); // Dump information about file onto standard error

    unsigned int  numStreams = player->pFormatCtx->nb_streams;
    AVStream**    streams    = player->pFormatCtx->streams;

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

    return player;
}

#include <cstdlib>

void mediaDestroyPlayer(media_player_t player)
{
    closeAudioStream(player);
    closeVideoStream(player);

    av_close_input_file(player->pFormatCtx);
}

void mediaStartPlayback(media_player_t player)
{
    player->timeOfNextFrame = 0;
    player->streamEnd       = FALSE;

    decodeAudio(player, 0);
    decodeFrame(player, 0);

    player->playback  = TRUE;
    player->timeShift = player->timeOfNextFrame; //in some videos first dts differs from 0
    player->baseTime  = timerAbsoluteTime();
}

void mediaPlayerUpdate(media_player_t player)
{
    if (!player->playback)
        return;

    __int64 currentTime = timerAbsoluteTime()-player->baseTime;

    decodeAudio(player, currentTime);
    decodeFrame(player, currentTime+player->timeShift);
}

void mediaPlayerPrepareRender(media_player_t player)
{
    glUseProgram(progYUV2RGB);

    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, player->texY[0]);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, player->texU[0]);
    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, player->texV[0]);

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