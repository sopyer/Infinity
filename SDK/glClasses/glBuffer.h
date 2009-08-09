template<GLenum Target>
class Buffer
{
	public:
		Buffer() {glGenBuffers(1, &mHandle);}
		~Buffer() {glDeleteBuffers(1, &mHandle);}

		void setData(GLsizeiptr size, GLvoid* data, GLenum usage=GL_STREAM_DRAW)
		{
			glBindBuffer(Target, mHandle);
			glBufferData(Target, size, data, usage);
			glBindBuffer(Target, 0);
		}

		GLvoid* map(GLenum mode=GL_WRITE_ONLY)
		{
			glBindBuffer(Target, mHandle);
			return glMapBuffer(Target, mode);
		}

		void unmap()
		{
			glBindBuffer(Target, mHandle);
			glUnmapBuffer(Target);
		}

	public:
		GLuint	mHandle;
};

typedef Buffer<GL_ARRAY_BUFFER> AttribBuffer;
typedef Buffer<GL_ELEMENT_ARRAY_BUFFER> IndexBuffer;