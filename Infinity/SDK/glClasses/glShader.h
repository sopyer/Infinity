class Shader: public Handle
{
	public:
		Shader(GLuint handle): Handle(handle) {}

		void compile(const GLchar* source)
		{
			GLint len = (GLint)strlen(source);
			glShaderSource(*this, 1, (const GLchar **)&source, &len);
			glCompileShader(*this); 
		}

		GLint getParam(GLenum pname)
		{
			GLint param = 0;
			glGetShaderiv(*this, pname, &param);
			return param;
		}

		void getLog(GLsizei maxLength, GLsizei *length, GLchar *infoLog)
		{
			glGetShaderInfoLog(*this, maxLength, length, infoLog);
		}
};

template <GLenum Type>
class ShaderImpl: public Shader
{
	public:
		ShaderImpl() : Shader(glCreateShader(Type))	{}
		~ShaderImpl() {glDeleteShader(*this);}
};

typedef ShaderImpl<GL_VERTEX_SHADER>	VertexShader;
typedef ShaderImpl<GL_FRAGMENT_SHADER>	FragmentShader;

class Program : public Handle
{
	public:
		Program() : Handle(glCreateProgram()) {}
		~Program() {glDeleteProgram(*this);}

		void addShader(GLuint handle) {glAttachShader(*this, handle);}

		void addShader(Shader* shader) {if (shader) addShader((GLuint)*shader);}
		void addShader(Shader& shader) {addShader(&shader);}
		
		void link()	{glLinkProgram(*this);}
		void validate()	{glValidateProgram(*this);}

		GLint getParam(GLenum pname)
		{
			GLint param = 0;
			glGetProgramiv(*this, pname, &param);
			return param;
		}

		//! Refactor this - change interface
		void getLog(GLsizei maxLength, GLsizei *length, GLchar *log)
		{
			glGetProgramInfoLog(*this, maxLength, length, log);
		}

		GLint bindUniform(const GLchar* name) {return glGetUniformLocation(*this, name);}
		GLint bindAttribute(const GLchar* name) {return glGetAttribLocation(*this, name);}
};
