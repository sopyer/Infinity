class Handle
{
	public:
		Handle() {}
		Handle(GLuint handle) : mHandle(handle) {}

		operator GLuint() {return mHandle;}

	private:
		GLuint mHandle;
};