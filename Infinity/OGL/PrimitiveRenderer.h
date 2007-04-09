#pragma once

#include <gl\glew.h>
#include "buffer.h"
#include "pipeline.h"
#include "texture.h"

enum
{
	USAGE_POSITION,
	USAGE_COLOR,
	USAGE_NORMAL,
	USAGE_TEXCOO0,
	USAGE_TEXCOO1,
	USAGE_TEXCOO2,
	USAGE_TEXCOO3,
	USAGE_ATTRIB_BASE
};
#define USAGE_ATTRIB(attr_loc) (attr_loc+USAGE_ATTRIB_BASE)

struct VertexDecl
{
	int Usage, ElCount, ElType, Offset, Stride;
};

#define BUFFER_OFFSET(i) ((char*)0 + (i))

//Potential bugs:
//Some ClientStates Will ramaind or will be disabled
//Check Texture Usage

class PrimitiveRenderer
{
private:
	static int ClientStatesMap[];
	int ClientStates[64/*128*/];
	int NumStates;
	bool IndSetted;
	GLenum IndType;
	GLuint Buf;
public:
	PrimitiveRenderer():NumStates(0), Buf(0), IndType(0), IndSetted(false){}
	void NewVertexStream()
	{
		for(int i=0; i<NumStates; i++)
		{
			if( ClientStates[i]>=USAGE_ATTRIB_BASE )
				glDisableVertexAttribArray(ClientStates[i] - USAGE_ATTRIB_BASE);
			else if( ClientStates[i]>=USAGE_TEXCOO0 )
			{
				glClientActiveTexture(GL_TEXTURE0_ARB+ClientStates[i]-USAGE_TEXCOO0);
				glDisableClientState( GL_TEXTURE_COORD_ARRAY );
			}
			else
				glDisableClientState(ClientStatesMap[ClientStates[i]]);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		IndSetted=false;
		NumStates=0;
	}
	
	void AddVertexAttrStream(BufferObject* Stream, int NumEntries, VertexDecl *Decl)
	{
		IndSetted=false;
		glBindBuffer(GL_ARRAY_BUFFER, Stream->GetHandle());
		for(int i=0; i<NumEntries; i++)
		{
			ClientStates[NumStates]=Decl[i].Usage;
			++NumStates;
			if( Decl[i].Usage >=USAGE_ATTRIB_BASE )
			{
				GLuint attrIndex = Decl[i].Usage - USAGE_ATTRIB_BASE;
				glEnableVertexAttribArray(attrIndex);
				glVertexAttribPointer(attrIndex, Decl[i].ElCount, Decl[i].ElType, GL_FALSE, 
									Decl[i].Stride, BUFFER_OFFSET(Decl[i].Offset));
			}
			else if( Decl[i].Usage>=USAGE_TEXCOO0 )
			{
				glClientActiveTexture(GL_TEXTURE0_ARB+Decl[i].Usage-USAGE_TEXCOO0);
				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
				glTexCoordPointer(Decl[i].ElCount, Decl[i].ElType, Decl[i].Stride, BUFFER_OFFSET(Decl[i].Offset));
			}
			else if( Decl[i].Usage==USAGE_POSITION )
			{
				glEnableClientState(ClientStatesMap[Decl[i].Usage]);
				glVertexPointer(Decl[i].ElCount, Decl[i].ElType, Decl[i].Stride, BUFFER_OFFSET(Decl[i].Offset));
			}
			else if( Decl[i].Usage==USAGE_COLOR )
			{
				glEnableClientState(ClientStatesMap[Decl[i].Usage]);
				glColorPointer(Decl[i].ElCount, Decl[i].ElType, Decl[i].Stride, BUFFER_OFFSET(Decl[i].Offset));
			}
			else if( Decl[i].Usage==USAGE_NORMAL )
			{
				//assert Decl[i].ElCount==3
				glEnableClientState(ClientStatesMap[Decl[i].Usage]);
				glNormalPointer(Decl[i].ElType, Decl[i].Stride, BUFFER_OFFSET(Decl[i].Offset));
			}
		}
	}

	void SetIndexStream(BufferObject* Stream, GLenum Type)
	{
		Buf = Stream->GetHandle();
		IndType = Type;
		IndSetted=false;
	}

	void DrawElements(GLenum Type, GLuint Count)
	{
		if( !IndSetted )
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buf);
			IndSetted = true;
		}
		glDrawElements(Type, Count, IndType, 0);
	}
	void SetProgram(GPUProgram* program)
	{
		static GLuint po = 0;
		NewVertexStream();
		if( program==0 )
		{
			po=0;
			glUseProgram(0);
		}
		else
		{
			GLuint cur=program->GetProgram();
			if( cur!=po )
			{
				po = cur;
				glUseProgram(po);
			}
		}
	}
	void SetTexture(int stage, Texture* tex)
	{
		glActiveTexture(GL_TEXTURE0+stage);
		if( tex!=0 )
		{
			//glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, tex->TextureID);
		}
		else
		{
			//glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
};