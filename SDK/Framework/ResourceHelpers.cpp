#include <algorithm>

#include "ResourceHelpers.h"
#include "Framework.h"
#include "SOIL.h"

#define MAX_DEFINES_TO_PROCESS 15

namespace resources
{
    GLuint createShaderFromFile(GLenum shaderType, const char* filePath, size_t definesCount, const char** defines)
    {
        File file = VFS::openRead(filePath);
        if (file)
        {
            GLint lens[MAX_DEFINES_TO_PROCESS+1];
            GLint len = (GLint)file.size();

            char* srcShader = new char[len+1];
            file.read(srcShader, len, 1);
            srcShader[len]=0;

            file.close();

            char* sources[MAX_DEFINES_TO_PROCESS+1];
            size_t sourceCount = std::min<size_t>(definesCount, MAX_DEFINES_TO_PROCESS);
            memcpy(sources, defines, sizeof(char*)*sourceCount);
            for (size_t i=0; i<sourceCount; ++i)
                lens[i] = strlen(sources[i]);
            sources[sourceCount] = srcShader;
            lens[sourceCount] = len;
            sourceCount++;

            GLuint shader = glCreateShader(shaderType);
            glShaderSource(shader, sourceCount, (const GLchar**)sources, lens);
            //glShaderSource(shader, 1, (const GLchar **)&source, &len);
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

            delete [] srcShader;

            return shader;
        }
        else
            logging::message("Failed to open file: %s\n", filePath);

        return 0;
    }

    GLuint createProgramFromFiles(const char* vertShaderPath, const char* fragShaderPath, size_t definesCount, const char** defines)
    {
        GLuint program = glCreateProgram();

        if (vertShaderPath)
        {
            GLuint vertShader = createShaderFromFile(GL_VERTEX_SHADER, vertShaderPath, definesCount, defines);
            glAttachShader(program, vertShader);
            glDeleteShader(vertShader);
        }

        if (fragShaderPath)
        {
            GLuint fragShader = createShaderFromFile(GL_FRAGMENT_SHADER, fragShaderPath, definesCount, defines);
            glAttachShader(program, fragShader);
            glDeleteShader(fragShader);
        }

        glLinkProgram(program);

        const size_t	LOG_STR_LEN = 1024;
        char			infoLog[LOG_STR_LEN] = {0};
        GLsizei			length;
        glGetProgramInfoLog(program, LOG_STR_LEN-1, &length, infoLog);
        infoLog[length] = 0;

        if (infoLog[0])
        {
            logging::message("%s\n", infoLog);
        }

        return program;
    }

    GLuint createProgramFromFiles(const char* vertShaderPath, const char* geomShaderPath, const char* fragShaderPath, size_t definesCount, const char** defines)
    {
        GLuint program = glCreateProgram();

        if (vertShaderPath)
        {
            GLuint vertShader = createShaderFromFile(GL_VERTEX_SHADER, vertShaderPath, definesCount, defines);
            glAttachShader(program, vertShader);
            glDeleteShader(vertShader);
        }

        if (geomShaderPath)
        {
            GLuint fragShader = createShaderFromFile(GL_GEOMETRY_SHADER, geomShaderPath, definesCount, defines);
            glAttachShader(program, fragShader);
            glDeleteShader(fragShader);
        }

        if (fragShaderPath)
        {
            GLuint fragShader = createShaderFromFile(GL_FRAGMENT_SHADER, fragShaderPath, definesCount, defines);
            glAttachShader(program, fragShader);
            glDeleteShader(fragShader);
        }

        glLinkProgram(program);

        const size_t	LOG_STR_LEN = 1024;
        char			infoLog[LOG_STR_LEN] = {0};
        GLsizei			length;
        glGetProgramInfoLog(program, LOG_STR_LEN-1, &length, infoLog);
        infoLog[length] = 0;

        if (infoLog[0])
        {
            logging::message("%s\n", infoLog);
        }

        return program;
    }

#define MAX_INFOLOG_LENGTH 4096
    bool compileAndAttachShader(GLuint program, GLenum type, GLsizei len, const char* source)
    {
        GLint	status, length;
        char	logStr[MAX_INFOLOG_LENGTH];

        GLuint	shader = glCreateShader(type);

        glShaderSource(shader, 1, &source, &len);
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        if (!status)
        {
            glGetShaderInfoLog(shader, MAX_INFOLOG_LENGTH, &length, logStr);
            printf("%s\n", logStr);
            return false;
        }

        glAttachShader(program, shader);
        glDeleteShader(shader);

        return true;
    }

    GLuint createProgram(GLenum type, const char* source)
    {
        GLchar	logStr[MAX_INFOLOG_LENGTH];
        GLint	status, length;

        GLuint program = glCreateProgram();

        compileAndAttachShader(program, type, strlen(source), source);

        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &status);

        if (!status)
        {
            glGetProgramInfoLog(program, MAX_INFOLOG_LENGTH, &length, logStr);
            printf("%s", logStr);
            glDeleteProgram(program);

            return 0;
        }

        glValidateProgram(program);
        glGetProgramiv(program, GL_VALIDATE_STATUS, &status);

        if (!status)
        {
            glGetProgramInfoLog(program, MAX_INFOLOG_LENGTH, &length, logStr);
            printf("%s", logStr);
        }

        return program;
    }

    GLuint linkProgram(GLsizei shaderCount, ...)
    {
        GLuint program;
        va_list arg;

        va_start(arg, shaderCount);
        program = glCreateProgram();

        while (shaderCount--)
        {
            GLuint shader = va_arg(arg, GLuint);
            glAttachShader(program, shader);
        }

        glLinkProgram(program);

#ifdef _DEBUG
        GLint	status;
        char output[8096];
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        glGetProgramInfoLog(program, 8096, &status, output);
        assert(status);
#endif
        va_end(arg);

        return program;
    }

    void getUniforms(GLuint program, GLsizei uniformCount, const char** uniformNames, GLint* uniforms)
    {
        for (GLsizei u=0; u<uniformCount; ++u)
        {
            uniforms[u] = glGetUniformLocation(program, uniformNames[u]);
            assert(uniforms[u]!=-1);
        }
    }

    GLuint createTexture2D(const char* name, GLint minFilter, GLint magFilter, GLint genMipmap)
    {
        File	src = VFS::openRead(name);

        //explicit conversion to avoid warning on 32-bit system
        assert(src.size()<SIZE_MAX);
        size_t fileSize = (size_t)src.size();

        unsigned char* data = new unsigned char[fileSize+1];

        src.read(data, fileSize, 1);

        GLuint texture;

        glGenTextures(1, &texture);

        int	imgWidth, imgHeight, imgChannels;
        unsigned char*	pixelsPtr = SOIL_load_image_from_memory(data, fileSize,
            &imgWidth, &imgHeight, &imgChannels, SOIL_LOAD_RGBA);

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, genMipmap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

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

        uint8_t* p = new uint8_t[w*h*4];
        glReadPixels (x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, p);
        SOIL_save_image(imageName, SOIL_SAVE_TYPE_TGA, w, h, 4, p);		
        delete [] p;
    }

    GLuint createRenderbuffer(GLenum type, GLsizei width, GLsizei height)
    {
        GLuint rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, type, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        GLenum err = glGetError();
        assert(err==GL_NO_ERROR);

        return rbo;
    }
}
