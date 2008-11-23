template <GLenum Type>
class Shader: public Handle
{
	public:
		Shader() : Handle(glCreateShader(Type))	{}
		~Shader() {glDeleteShader(*this);}

		void compile(const GLchar* source)
		{
			GLint length = (GLint)strlen(source);
			glShaderSource(*this, 1, (const GLchar **)&source, &length);
			glCompileShader(*this); 
		}

		GLint getParam(GLenum pname)
		{
			GLint param = 0;
			glGetShaderiv(*this, pname, &param);
			return param;
		}

		void getInfoLog(GLsizei maxLength, GLsizei *length, GLchar *infoLog)
		{
			glGetShaderInfoLog(*this, maxLength, length, infoLog);
		}
};

typedef Shader<GL_VERTEX_SHADER>	VertexShader;
typedef Shader<GL_FRAGMENT_SHADER>	FragmentShader;

class Program : public Handle
{
	public:
		Program() : Handle(glCreateProgram()) {}
		~Program() {glDeleteProgram(*this);}

		void addShader(GLuint handle) {glAttachShader(*this, handle);}

		void addShader(Handle* shader) {if (shader) addShader((GLuint)*shader);}
		void addShader(Handle& shader) {addShader(&shader);}
		
		void link()	{glLinkProgram(*this);}
		void validate()	{glValidateProgram(*this);}

		GLint getParam(GLenum pname)
		{
			GLint param = 0;
			glGetProgramiv(*this, pname, &param);
			return param;
		}

		void getInfoLog(GLsizei maxLength, GLsizei *length, GLchar *infoLog)
		{
			glGetProgramInfoLog(*this, maxLength, length, infoLog);
		}

		GLint getUniformLocation(const GLchar* name) {return glGetUniformLocation(*this, name);}
		GLint getAttribLocation(const GLchar* name) {return glGetAttribLocation(*this, name);}
};
