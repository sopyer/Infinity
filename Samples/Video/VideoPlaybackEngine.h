#ifndef __VIDEOPLAYBACKENGINE_H_INCLUDED__
#	define __VIDEOPLAYBACKENGINE_H_INCLUDED__

#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <cstdio>
#include <gl/glee.h>
#include <al/al.h>
#include <vector>

extern "C"
{
	#include <avcodec.h>
	#include <avformat.h>
	#include <swscale.h> // include the header!
}

class VideoPlaybackEngine
{
	public:
		void init();
		void cleanup();
		void decodeVideoTask(GLuint texture, ALuint buffer, unsigned int& frameTime);
		void updateGLTexture(GLuint texture);
		void updateALBuffer(ALuint buffer);

	private:
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