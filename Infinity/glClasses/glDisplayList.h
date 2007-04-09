class glDisplayList
{
	public:
		glDisplayList()
		{
			handle_ = glGenLists(1);
		}
		~glDisplayList()
		{
			glDeleteLists(handle_, 1);
		}
		void begin()
		{
			glNewList(handle_, GL_COMPILE);
		}
		void end()
		{
			glEndList();
		}

	public:
		GLuint handle_;
};