#include "Loader.h"

#define move(sz) data+=sz
#define read_uint32(var) var=*(uint32*)data;data+=4

static VertexDecl BVCD[]=
{
	{USAGE_POSITION, 3, GL_FLOAT, 0, 3*4},
	{USAGE_NORMAL, 3, GL_FLOAT, 0, 3*4},
	{USAGE_TEXCOO0, 2, GL_FLOAT, 0, 2*4}
};

static VertexDecl TD[]=
{
	{USAGE_POSITION, 3, GL_FLOAT, 0, 3*4},
	{USAGE_TEXCOO0, 2, GL_FLOAT, 0, 2*4},
	{USAGE_NORMAL, 3, GL_FLOAT, 0, 3*4},
	{USAGE_ATTRIB_BASE, 3, GL_FLOAT, 0, 3*4},
	{USAGE_ATTRIB_BASE, 3, GL_FLOAT, 0, 3*4}
};

void Loader::Load(DataStreamPtr stream, GLuint tanattr, GLuint bitanattr)
{
	byte* data1 = stream->MapStream();
	byte* data=data1;
	uint32 nv1, nv2, vsz1, vsz2, t;

	move(8);
	read_uint32(t);
	read_uint32(nv1);
	
	if( t == 0x20 )
	{
		vsz1 = nv1*4*(3+2+3+3+3);
		Decl = TD;
		Decl[1].Offset = Decl[0].Offset+Decl[0].Stride*nv1;
		Decl[2].Offset = Decl[1].Offset+Decl[1].Stride*nv1;
		Decl[3].Offset = Decl[2].Offset+Decl[2].Stride*nv1;
		Decl[4].Offset = Decl[3].Offset+Decl[3].Stride*nv1;
		Decl[3].Usage = USAGE_ATTRIB(tanattr);
		Decl[4].Usage = USAGE_ATTRIB(bitanattr);
		num_decl = 5;
	}

	vert_buf = new VertexAttrBuffer(vsz1);
	vert_buf->SetBufferData(vsz1, data);
	move(vsz1);
	
	uint32 ni1;
	read_uint32(ni1);
	ind_buf = new IndexBuffer(ni1*2);
	ind_buf->SetBufferData(ni1*2, data);
	num_ind = ni1;
}

//void Loader::Load(DataStreamPtr stream)
//{
//	byte* data1 = stream->MapStream();
//	byte* data=data1;
//	uint32 nv1, nv2, vsz1, vsz2, t;
//
//	move(5);
//	//read_uint32(nt1);
//	//read_uint32(nt2);
//	move(8);
//	move(4);
//	read_uint32(nv1);
//	read_uint32(t);
//	
//	if( t == 0x25 )
//	{
//		vsz1 = nv1*4*8;
//		Decl = BVCD;
//		Decl[1].Offset = Decl[0].Stride*nv1;
//		Decl[2].Offset = Decl[1].Offset+Decl[1].Stride*nv1;
//	}
//	//else
//	//{
//	//	isz1 = nt1*3*
//	//}
//
//	vert_buf = new VertexAttrBuffer(vsz1);
//	vert_buf->SetBufferData(vsz1, data);
//	move(vsz1);
//
//	read_uint32(nv2);
//	read_uint32(t);
//	
//	if( t == 0x25 )
//	{
//		vsz2 = nv2*4*8;
//		num_decl=3;
//	}
//	else
//	{
//		vsz2 = nv2*4*(8+4);
//	}
//
//	move(vsz2);
//	read_uint32(t);
//	for(uint32 i=0; i<t; i++)
//	{
//		int t1;
//		read_uint32(t1);
//		move(t1);
//	}
//	
//	uint32 ni1;
//	read_uint32(ni1);
//	ind_buf = new IndexBuffer(ni1*2);
//	ind_buf->SetBufferData(ni1*2, data);
//	num_ind = ni1;
//}

