#include "glClasses.h"
#include <malloc.h>
#include <cassert>
#include <algorithm>

namespace gl
{
	struct UniformDesc
	{
		const char* name;
		GLint		size;
		GLenum		type;
		GLint		loc;
		GLvoid*		value;//make union!!!!
	};

	UniformTable::UniformTable(GLuint program)
	{
		parse(program);
	}

	UniformTable::~UniformTable()
	{
		if (mDesc)
			free(mDesc);
	}

	GLuint getSizeOfType(GLenum type)
	{
		switch (type)
		{
			case GL_FLOAT:
				return sizeof(GLfloat);
			case GL_FLOAT_VEC2:
				return 2*sizeof(GLfloat);
			case GL_FLOAT_VEC3:
				return 3*sizeof(GLfloat);
			case GL_FLOAT_VEC4:
				return 4*sizeof(GLfloat);

			case GL_INT:
				return sizeof(GLuint);
			case GL_INT_VEC2:
				return 2*sizeof(GLuint);
			case GL_INT_VEC3:
				return 3*sizeof(GLuint);
			case GL_INT_VEC4:
				return 4*sizeof(GLuint);

			case GL_BOOL:
				return sizeof(GLuint);
			case GL_BOOL_VEC3:
				return 2*sizeof(GLuint);
			case GL_BOOL_VEC2:
				return 3*sizeof(GLuint);
			case GL_BOOL_VEC4:
				return 4*sizeof(GLuint);

			case GL_FLOAT_MAT2:
				return 2*2*sizeof(GLfloat);
			case GL_FLOAT_MAT3:
				return 3*3*sizeof(GLfloat);
			case GL_FLOAT_MAT4:
				return 4*4*sizeof(GLfloat);

			case GL_SAMPLER_1D:
			case GL_SAMPLER_2D:
			case GL_SAMPLER_3D:
			case GL_SAMPLER_CUBE:
			case GL_SAMPLER_1D_SHADOW:
			case GL_SAMPLER_2D_SHADOW:
				return sizeof(GLuint);
			default:
				assert(0);
		}

		return 0;
	}

	void UniformTable::parse(GLuint program)
	{
		GLint mMaxUniformNameLength;

		glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &mNumUniforms);
		glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &mMaxUniformNameLength);

		size_t			dataSize = 0;
		size_t			descSize = mNumUniforms*sizeof(UniformDesc);
		size_t			nameSize = 0;
		size_t			maxNameSize = mNumUniforms*(mMaxUniformNameLength+1);

		GLubyte*		data    = (GLubyte*)alloca(descSize+maxNameSize);
		UniformDesc*	desc    = (UniformDesc*)data;
		char*			strings = (char*)(data+mNumUniforms*sizeof(UniformDesc));
		char*			strPtr  = strings;

		for(GLint i=0; i<mNumUniforms; ++i)
		{
			GLsizei	len;

			glGetActiveUniform(program, i, mMaxUniformNameLength, &len,
					&desc[i].size, &desc[i].type, strPtr);

			desc[i].name  = strPtr;
			desc[i].value = (void*)dataSize;
			desc[i].loc   = glGetUniformLocation(program, strPtr);

			//skip built in uniforms
			if (desc[i].loc != -1)
				dataSize += getSizeOfType(desc[i].type)*desc[i].size;
			else
				desc[i].value = 0;
			
			strPtr += len+1;
		}
		
		nameSize = strPtr-strings;
		GLubyte*	mem = (GLubyte*)malloc(descSize+nameSize+dataSize);
		mDesc = (UniformDesc*)mem;

		memcpy(mem, data, descSize+nameSize);

		const char* newStrPtr = (const char*)mem+descSize;
		GLubyte*	dataPtr   = (GLubyte*)mem+descSize+nameSize;
		for (GLint i=0; i<mNumUniforms; ++i)
		{
			mDesc[i].name  = newStrPtr+(desc[i].name-strings);
			if (desc[i].loc != -1)
			{
				assert((size_t)mDesc[i].value+getSizeOfType(desc[i].type)*desc[i].size < descSize+nameSize+dataSize);
				mDesc[i].value = dataPtr+(size_t)mDesc[i].value;
				assert(mDesc[i].value>=dataPtr && mDesc[i].value<dataPtr+dataSize);
			}
		}
	}

	void UniformTable::setUniform(const char* name, float x, float y, float z, float w)
	{
		struct SearchUniformByName
		{
			SearchUniformByName(const char* name): mName(name) {}
			bool operator()(const UniformDesc& desc) {return strcmp(mName, desc.name)==0;}

			const char* mName;
		};

		UniformDesc* begin = mDesc;
		UniformDesc* end = mDesc+mNumUniforms;
		UniformDesc* it;

		it = std::find_if(begin, end, SearchUniformByName(name));

		if (it!=end)
		{
			assert(it->type==GL_FLOAT_VEC4 && it->size==1);
			float (*data)[4] = (float (*)[4])it->value;

			(*data)[0] = x;
			(*data)[1] = y;
			(*data)[2] = z;
			(*data)[3] = w;
		}
	}
}