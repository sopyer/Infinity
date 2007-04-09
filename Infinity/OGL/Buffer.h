#pragma once

#include <gl\glew.h>

class BufferObject
{
	friend class PrimitiveRender;
public:
	BufferObject(int BufferSize, GLenum Target, GLenum Usage=GL_STREAM_DRAW):iSize(BufferSize), BufTarget(Target), BufUsage(Usage)
	{
		glGenBuffers(1, &BufferHandle);
		glBindBuffer(BufTarget, BufferHandle);
		glBufferData(BufTarget, iSize, 0, Usage);
	}
	virtual ~BufferObject()
	{
		glDeleteBuffers(1, &BufferHandle);
	}
	void SetBufferData(GLsizeiptr Size, GLvoid* Data, GLenum Usage=GL_STREAM_DRAW)
	{
		glBindBuffer(BufTarget, BufferHandle);
		glBufferData(BufTarget, Size, Data, Usage);
	}
	GLvoid* MapData(GLenum Mode=GL_WRITE_ONLY)
	{
		glBindBuffer(BufTarget, BufferHandle);
		return glMapBuffer(BufTarget, GL_WRITE_ONLY);
	}
	void UnmapData()
	{
		glBindBuffer(BufTarget, BufferHandle);
		glUnmapBuffer(BufTarget);
	}

	GLuint GetHandle(){return BufferHandle;}
private:
	int iSize;
	GLenum	BufTarget, BufUsage;
	GLuint	BufferHandle;
};

enum VertexUsage{VertexColor, VertexPosition, VertexNormal};
struct VertexDescEntry
{
	GLenum Type;
	int Size;
	int Usage;
};

class VertexAttrBuffer: public BufferObject
{
public:
	VertexAttrBuffer(int size, GLenum Usage=GL_STREAM_DRAW):BufferObject(size, GL_ARRAY_BUFFER, Usage){}
};

class IndexBuffer: public BufferObject
{
public:
	IndexBuffer(int size, GLenum Usage=GL_STREAM_DRAW):BufferObject(size, GL_ELEMENT_ARRAY_BUFFER, Usage){}
};