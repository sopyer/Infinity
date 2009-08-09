struct UniformDesc;

//Break into 2 classes: reflection and data storage
//Try integrate uniform buffers for data storage
class UniformTable
{
	public:
		UniformTable(GLuint program);
		~UniformTable();

		void setUniform(const char* name, float x, float y, float z, float w);

	private:
		void parse(GLuint program);

	private:
		GLint			mNumUniforms;
		GLint			mMaxUniformNameLength;
		UniformDesc*	mDesc;
};