#include <cstring>

class glShaderBase
{
	public:
		~glShaderBase()
		{
			glDeleteShader(handle_);
		}
		void compile(const GLchar* source)
		{
			GLint length = (GLint)strlen(source);
			glShaderSource(handle_, 1, (const GLchar **)&source, &length);
			glCompileShader(handle_); 
		}
		GLint getParam(GLenum pname)
		{
			GLint param = 0;
			glGetShaderiv(handle_, pname, &param);
			return param;
		}
		void getInfoLog(GLsizei maxLength, GLsizei *length, GLchar *infoLog)
		{
			glGetShaderInfoLog(handle_, maxLength, length, infoLog);
		}

	public:
		GLuint handle_;  //!< Shader Object
};

typedef glShaderBase glShader;

//-----------------------------------------------------------------------------

//! \ingroup GLSL
class glVertexShader : public glShaderBase
{
	public:
		glVertexShader()  //!< Constructor for Vertex Shader
		{
			handle_ = glCreateShader(GL_VERTEX_SHADER);
		}
};

//-----------------------------------------------------------------------------

//! \ingroup GLSL
class glFragmentShader : public glShaderBase
{
	public:
		glFragmentShader() //!< Constructor for Fragment Shader
		{
			handle_ = glCreateShader(GL_FRAGMENT_SHADER);
		}
};

class glProgram
{
	public:
		glProgram()
		{
			handle_ = glCreateProgram();
		}
		~glProgram()
		{
			glDeleteProgram(handle_);
		}
		void addShader(glShaderBase*	shader)
		{
			if( shader )
				glAttachShader(handle_, shader->handle_);//(shader)?shader->handle_:0
		}
		void link()
		{
			glLinkProgram(handle_);
		}
		void validate()
		{
			glValidateProgram(handle_);
		}
		GLint getParam(GLenum pname)
		{
			GLint param = 0;
			glGetProgramiv(handle_, pname, &param);
			return param;
		}
		void getInfoLog(GLsizei maxLength, GLsizei *length, GLchar *infoLog)
		{
			glGetProgramInfoLog(handle_, maxLength, length, infoLog);
		}
		GLint getUniformLocation(const GLchar* name)
		{
			return glGetUniformLocation(handle_, name);
		}

		GLint getAttribLocation(const GLchar* name)
		{
			return glGetAttribLocation(handle_, name);
		}

	public:
		GLuint	handle_;
};