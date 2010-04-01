#ifndef __VIDEOPLAYBACKENGINE_H_INCLUDED__
#	define __VIDEOPLAYBACKENGINE_H_INCLUDED__

#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <cstdio>
#include <gl/glee.h>

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
		void decodeVideoTask(GLuint texture, unsigned int& frameTime);
		void updateGLTexture(GLuint texture);

	private:
		AVFormatContext *pFormatCtx;
		int             videoStream;
		AVCodecContext  *pCodecCtx;
		AVCodec         *pCodec;
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