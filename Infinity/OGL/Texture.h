#pragma once

#include <gl\glew.h>
#include <windows.h>
#include <ijl.h>
#include "types.h"

//Base class like BufferObject add support for Texture1D, Texture3D 4D????
class Texture
{
public:
	Texture(void)
	{
		Bpp=0;
		Width=0;
		Height=0;
		glGenTextures(1, &TextureID);
	}
	
	void Bind(GLuint Stage)
	{
		glActiveTexture(GL_TEXTURE0+Stage);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, TextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	
				//GLenum gl=glGetError();
				//gl = gl+1-1;
	}

	~Texture(void){glDeleteTextures(1, &TextureID);}

	BYTE Bpp;
	UINT Width;
	UINT Height;
	UINT TextureID;
};

#if 0
class Texture
{
public:
	Texture(GLenum target, GLint internalFmt, GLint minFilter, GLint magFilter, bool forceMipmap=true)
	{
		internalFormat = internalFmt;
		texTarget = target;
		glGenTextures(1, &texID);
		glBindTexture(target, texID);
	    glTexParameteri(target, GL_GENERATE_MIPMAP, forceMipmap);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);

	}
	
	~Texture(void){glDeleteTextures(1, &TextureID);}
	
	GLenum	texTarget;
	GLuint	texID;
	GLint	internalFormat;
};

class Texture2D: public Texture
{
public:
	Texture2D(GLint internalFmt, GLint minFilter, GLint magFilter, bool forceMipmap=true):
	  Texture(GL_TEXTURE_2D, internalFmt, minFilter, magFilter, forceMipmap)
	
	void setWrapMode(GLint wrapS, GLint wrapT)
	{
		glBindTexture(texTarget, texID);
		glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapT);
	}

	void setImage2D(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)//maybe this is not usefullas
	{
		glBindTexture(texTarget, texID);
		glTexImage2D(texTarget, 0, internalFormat, width, height, 0, format, type, pixels);
	}
};

#endif
