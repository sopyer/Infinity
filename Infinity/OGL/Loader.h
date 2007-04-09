#pragma once
#include "vfs.h"
#include "Buffer.h"
#include "types.h"
#include "PrimitiveRenderer.h"

class Loader
{
public:
	void Load(DataStreamPtr stream, GLuint tanattr, GLuint bitanattr);

	void Unload()
	{
		delete vert_buf;
		delete ind_buf;
	}

	BufferObject* vert_buf;
	BufferObject* ind_buf;
	VertexDecl* Decl;
	int num_decl, num_ind;
};