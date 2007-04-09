#ifndef _LOADING_H_INCLUDED_
#	define _LOADING_H_INCLUDED_

glShader* loadShader(const char* path);
glProgram* loadProgram(const char* shaders);
glTexture2D* loadTargaTexture(const char* path);
glTexture2D* loadJpegTexture(const char* path);
glTexture2D* loadPngTexture(const char* path);

#endif