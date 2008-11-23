class DisplayList
{
	public:
		DisplayList()
		{
			mHandle = glGenLists(1);
		}
		~DisplayList()
		{
			glDeleteLists(mHandle, 1);
		}
		void begin()
		{
			glNewList(mHandle, GL_COMPILE);
		}
		void end()
		{
			glEndList();
		}

	public:
		GLuint mHandle;
};