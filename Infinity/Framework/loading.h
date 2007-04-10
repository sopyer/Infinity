#ifndef _LOADING_H_INCLUDED_
#	define _LOADING_H_INCLUDED_

#include <boost\shared_ptr.hpp>

typedef boost::shared_ptr<glShader> glShaderPtr;
typedef boost::shared_ptr<glProgram> glProgramPtr;
typedef boost::shared_ptr<glTexture2D> glTexture2DPtr;

glShader* loadShader(const char* path);
glProgram* loadProgram(const char* shaders);
glTexture2D* loadTargaTexture(const char* path);
glTexture2D* loadJpegTexture(const char* path);
glTexture2D* loadPngTexture(const char* path);

#endif