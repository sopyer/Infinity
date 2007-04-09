class glTexture
{
	public:
		glTexture(GLenum target, GLint internalFmt, GLint minFilter, GLint magFilter, bool forceMipmap=true)
		{
			internalFormat_ = internalFmt;
			target_ = target;
			glGenTextures(1, &handle_);
			glBindTexture(target, handle_);
			glTexParameteri(target, GL_GENERATE_MIPMAP, forceMipmap);
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);

		}
		~glTexture(void){glDeleteTextures(1, &handle_);}
		
	public:
		GLenum	target_;
		GLuint	handle_;
		GLint	internalFormat_;
};

class glTexture2D: public glTexture
{
	public:
		glTexture2D(GLint internalFmt, GLint minFilter, GLint magFilter, bool forceMipmap=true):
		  glTexture(GL_TEXTURE_2D, internalFmt, minFilter, magFilter, forceMipmap){}

		void setWrapMode(GLint wrapS, GLint wrapT)
		{
			glBindTexture(target_, handle_);
			glTexParameteri(target_, GL_TEXTURE_WRAP_S, wrapS);
			glTexParameteri(target_, GL_TEXTURE_WRAP_T, wrapT);
		}

		void setImage2D(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)//maybe this is not usefullas
		{
			glBindTexture(target_, handle_);
			glTexImage2D(target_, 0, internalFormat_, width, height, 0, format, type, pixels);
		}
};