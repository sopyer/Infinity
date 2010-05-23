#ifndef __VIDEOPLAYBACKENGINE_H_INCLUDED__
#	define __VIDEOPLAYBACKENGINE_H_INCLUDED__

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <cstdio>
#include <gl/glee.h>
#include <al/al.h>
#include <al/alc.h>
#include <vector>
#include <scheduler.h>
#include <queue>
#include <context.h>

extern "C"
{
	#include <avcodec.h>
	#include <avformat.h>
	#include <swscale.h> // include the header!
}

class VideoPlaybackEngine
{
	public:
		void init(GLsizei numTextures, GLuint* textures, ALsizei numBuffers, ALuint* buffers);
		void init(ctx::Context context);
		void createResources();
		void destroyResources();
		void cleanup();
		void decodeVideoTask(GLuint texture, ALuint buffer, unsigned int& frameTime);
		void updateGLTexture(GLuint texture);
		void updateALBuffer(ALuint buffer);

		void decodeThreadFunc();

		void getFrame(GLuint& texture, ALuint& buffer, unsigned int& frameTime);

	private:
		mt::Thread	mDecodeThread;
		mt::Mutex	mMutex;
		bool				mQuitDecodeThread;
		bool				mIsStreaming;
		
		ctx::Context		mResCtx;

#ifdef _WIN32
		HDC	mHDC;
#endif

		std::queue<GLuint>	mFreeTextures;
		std::queue<ALuint>	mFreeBuffers;

		std::queue<GLuint>			mDecodedTextures;
		std::queue<ALuint>			mDecodedBuffers;
		std::queue<unsigned int>	mFrameTimes;

		int					mAudioFormat;
		std::vector<char>	mAudioData;

		AVFormatContext *pFormatCtx;
		int             videoStream, audioStream;
		AVCodecContext  *pVideoCodecCtx;
		AVCodecContext  *pAudioCodecCtx;
		//AVCodec         *pVideoCodec;
		AVFrame         *pFrame; 
		AVFrame         *pFrameRGB;
		int             numBytes;
		int				width, height;
		uint8_t         *buffer;
		PixelFormat		srcFormat, dstFormat;
		SwsContext*		img_convert_ctx;
		double			timeBase;
};

#endif