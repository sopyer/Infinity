#ifndef __FRAMEWORK_RESOURCEHELPERS_H_INCLUDED__
#	define __FRAMEWORK_RESOURCEHELPERS_H_INCLUDED__

#include <gl/glee.h>

namespace resources
{
	GLuint createShaderFromFile(GLenum shaderType, const char* filePath);
	GLuint createProgramFromFiles(const char* vertShaderPath, const char* fragShaderPath);
	
	bool	compileAndAttachShader(GLuint program, GLenum type, GLsizei len, const char* source);
	GLuint	createProgram(GLenum type, const char* source);

	GLuint createTexture2D(const char* name);
	GLuint createSpecialTexture1D(GLint internalformat, GLsizei width,
								  GLenum format, GLenum type, const GLvoid *pixels);


	void saveScreenToFile(const char* imageName);
}

#endif