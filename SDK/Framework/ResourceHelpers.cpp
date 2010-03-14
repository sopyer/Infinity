#include "ResourceHelpers.h"
#include "Framework.h"
#include "SOIL.h"

namespace resources
{
	GLuint createShaderFromFile(GLenum shaderType, const char* filePath)
	{
		File file = VFS::openRead(filePath);
		if (file)
		{
			GLint len = (GLint)file.size();
			
			char* source = new char[len+1];
			file.read(source, len, 1);
			source[len]=0;

			GLuint shader = glCreateShader(shaderType);
			glShaderSource(shader, 1, (const GLchar **)&source, &len);
			glCompileShader(shader);
			
			//Add log output
			const size_t	LOG_STR_LEN = 1024;
			char			infoLog[LOG_STR_LEN] = {0};
			GLsizei			length;
			glGetShaderInfoLog(shader, LOG_STR_LEN-1, &length, infoLog);
			infoLog[length] = 0;
			
			if (infoLog[0])
			{
				logging::message("%s: %s\n", filePath, infoLog);
			}

			delete [] source;

			return shader;
		}
		else
			logging::message("Failed to open file: %s\n", filePath);

		return 0;
	}

	GLuint createProgramFromFiles(const char* vertShaderPath, const char* fragShaderPath)
	{
		GLuint vertShader = createShaderFromFile(GL_VERTEX_SHADER, vertShaderPath);
		GLuint fragShader = createShaderFromFile(GL_FRAGMENT_SHADER, fragShaderPath);
		GLuint program = glCreateProgram();
		
		glAttachShader(program, vertShader);
		glAttachShader(program, fragShader);
		glLinkProgram(program);
		
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		return program;
	}

	GLuint createTexture2D(const char* name)
	{
		File	src = VFS::openRead(name);

		unsigned char* data = new unsigned char[src.size()+1];

		src.read(data, src.size(), 1);

		GLuint texture;

		glGenTextures(1, &texture);

		int	imgWidth, imgHeight, imgChannels;
		unsigned char*	pixelsPtr = SOIL_load_image_from_memory(data, src.size(),
			&imgWidth, &imgHeight, &imgChannels, SOIL_LOAD_RGBA);

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelsPtr);
		
		SOIL_free_image_data(pixelsPtr);

		delete [] data;

		return texture;
	}

	GLuint createSpecialTexture1D(GLint internalFormat, GLsizei width, 
								  GLenum format, GLenum type, const GLvoid *pixels)
	{
		GLuint texture;

		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_1D, texture);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexImage1D(GL_TEXTURE_1D, 0, internalFormat, width, 0, format, type, pixels);
		
		return texture;
	}

	void saveScreenToFile(const char* imageName)
	{
		GLint vp[4];

		glGetIntegerv(GL_VIEWPORT, vp);

		GLint x = vp[0], y = vp[1], w = vp[2], h = vp[3];

		u8* p = new u8[w*h*4];
		glReadPixels (x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, p);
		SOIL_save_image(imageName, SOIL_SAVE_TYPE_TGA, w, h, 4, p);		
		delete [] p;
	}
}