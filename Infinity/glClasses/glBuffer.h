template<GLenum target>
class glBufferTemplate
{
	public:
		GLuint	handle_;

	public:
		glBufferTemplate()
		{
			glGenBuffers(1, &handle_);
		}
		~glBufferTemplate()
		{
			glDeleteBuffers(1, &handle_);
		}
		void setBufferData(GLsizeiptr size, GLvoid* data, GLenum usage=GL_STREAM_DRAW)
		{
			glBindBuffer(target, handle_);
			glBufferData(target, size, data, usage);
			glBindBuffer(target, 0);
		}
		GLvoid* mapBuffer(GLenum mode=GL_WRITE_ONLY)
		{
			glBindBuffer(target, handle_);
			return glMapBuffer(target, mode);
		}
		void unmapBuffer()
		{
			glBindBuffer(target, handle_);
			glUnmapBuffer(target_);
		}
};

typedef glBufferTemplate<GL_ARRAY_BUFFER> glAttribBuffer;
typedef glBufferTemplate<GL_ELEMENT_ARRAY_BUFFER> glIndexBuffer;