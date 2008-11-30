#include "glClasses.h"

void gl::Context::addAttribBuffer(AttribBuffer* buffer, GLuint numEntries, VertexDecl *decl)
{
	if( !buffer )
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return;
	}
	glBindBuffer(GL_ARRAY_BUFFER, buffer->mHandle);
	for(GLuint i=0; i<numEntries; i++)
	{
		if( decl[i].usage_ >=USAGE_ATTRIB_BASE )
		{
			GLuint attrIndex = decl[i].usage_ - USAGE_ATTRIB_BASE;
			glEnableVertexAttribArray(attrIndex);
			glVertexAttribPointer(attrIndex, decl[i].elCount_, decl[i].elType_, GL_FALSE, 
								decl[i].stride_, BUFFER_OFFSET(decl[i].offset_));
		}
		else if( decl[i].usage_>=USAGE_TEXCOORD0 )
		{
			glClientActiveTexture(GL_TEXTURE0+(decl[i].usage_-USAGE_TEXCOORD0));
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			glTexCoordPointer(decl[i].elCount_, decl[i].elType_, decl[i].stride_, BUFFER_OFFSET(decl[i].offset_));
		}
		else if( decl[i].usage_==USAGE_POSITION )
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(decl[i].elCount_, decl[i].elType_, decl[i].stride_, BUFFER_OFFSET(decl[i].offset_));
		}
		else if( decl[i].usage_==USAGE_COLOR )
		{
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(decl[i].elCount_, decl[i].elType_, decl[i].stride_, BUFFER_OFFSET(decl[i].offset_));
		}
		else if( decl[i].usage_==USAGE_NORMAL )
		{
			//assert decl[i].elCount_==3
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(decl[i].elType_, decl[i].stride_, BUFFER_OFFSET(decl[i].offset_));
		}
		else if( decl[i].usage_==USAGE_SECONDARYCOLOR )
		{
			//assert decl[i].elCount_==3
			glEnableClientState(GL_SECONDARY_COLOR_ARRAY);
			glSecondaryColorPointer(decl[i].elCount_, decl[i].elType_, decl[i].stride_, BUFFER_OFFSET(decl[i].offset_));
		}
	}
}