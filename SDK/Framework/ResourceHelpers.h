#ifndef __FRAMEWORK_RESOURCEHELPERS_H_INCLUDED__
#	define __FRAMEWORK_RESOURCEHELPERS_H_INCLUDED__

#include <opengl.h>

namespace resources
{
	//Up to 15 headers, others are ignored
	GLuint createShaderFromFile  (GLenum shaderType, const char* filePath,
								  size_t headerCount=0, const char** headers=0);
	GLuint createProgramFromFiles(const char* vertShaderPath, const char* fragShaderPath,
								  size_t headerCount=0, const char** headers=0);
	GLuint createProgramFromFiles(const char* vertShaderPath, const char* geomShaderPath, const char* fragShaderPath,
								  size_t headerCount=0, const char** headers=0);
	GLuint createProgramFromFiles(size_t headerCount, const char** headers, size_t sourceCount, ...);

	bool	compileAndAttachShader(GLuint program, GLenum type, GLsizei len, const char* source);
	GLuint	createProgram(GLenum type, const char* source);
	GLuint	linkProgram(GLsizei shaderCount, ...);
	void	getUniforms(GLuint program, GLsizei uniformCount, const char** uniformNames, GLint* uniforms);

	GLuint createTexture2D(const char* name,
		GLint minFilter=GL_LINEAR_MIPMAP_LINEAR,
		GLint magFilter=GL_LINEAR,
		GLint genMipmap=GL_TRUE);

	GLuint createSpecialTexture1D(GLint internalformat, GLsizei width,
								  GLenum format, GLenum type, const GLvoid *pixels);


	GLuint createRenderbuffer(GLenum type, GLsizei width, GLsizei height);

	void saveScreenToFile(const char* imageName);
}

#endif