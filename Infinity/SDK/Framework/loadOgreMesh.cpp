#include "Framework.h"
#include <DataPtr.h>

#define OGRE_HEADER 0x1000
#define OGRE_MESH 0x3000

// sub chunks of OGRE_MESH
#define OGRE_SUBMESH 0x4000
#define OGRE_GEOMETRY 0x5000
#define OGRE_SKELETON_LINK 0x6000
#define OGRE_BONE_ASSIGNMENT 0x7000
#define OGRE_MESH_LOD 0x8000
#define OGRE_MESH_BOUNDS 0x9000
#define OGRE_MESH_SUBMESH_NAME_TABLE 0xA000
#define OGRE_MESH_EDGE_LISTS 0xB000

// sub chunks of OGRE_GEOMETRY
#define OGRE_GEOMETRY_VERTEX_DECLARATION 0x5100
#define OGRE_GEOMETRY_VERTEX_ELEMENT 0x5110
#define OGRE_GEOMETRY_VERTEX_BUFFER 0x5200
#define OGRE_GEOMETRY_VERTEX_BUFFER_DATA 0x5210

// sub chunks of OGRE_SUBMESH
#define OGRE_SUBMESH_OPERATION 0x4010
#define OGRE_SUBMESH_BONE_ASSIGNMENT 0x4100
#define OGRE_SUBMESH_TEXTURE_ALIAS 0x4200

typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned int uint;

#define RETURN(str) return false

void readFile(DataPtr& ptr);

static char string[256];

bool loadOgreMesh(const char* path)
{
	DataStreamPtr stream = vfsMapFile(path);
	if(!stream)
		RETURN("File does not exist");
	
	DataPtr ptr(stream->MapStream(), stream->GetSize());

	short id;
	ptr.read(id);
	
	if(id!=OGRE_HEADER)
		RETURN("WRONG FILE TYPE - NON NATIVE(WRONG ENDIANESS) OR NOT A MESH");
	ptr.readString(string, 256, '\n');

	readFile(ptr);

	return true;
}

void readMesh(DataPtr& ptr);
void readGeometry(DataPtr& ptr);
void readSubMesh(DataPtr& ptr);
void readGeometryVertexDeclaration(DataPtr& ptr);

void readFile(DataPtr& ptr)
{
	ushort	id;
	ulong	len;
	while( !ptr.end() )
	{
		ptr.read(id);
		ptr.read(len);
		if( id==OGRE_MESH )
		{
			DataPtr p(ptr.move(0), len-6);
			logMessage("OGRE_MESH\n");
			readMesh(p);
			ptr.move(p.tell());
		}
		else
			return;
	}
}

void readMesh(DataPtr& ptr)
{
	ushort	id;
	ulong	len;
	ptr.move(1);
	while( !ptr.end() )
	{
		ptr.read(id);
		ptr.read(len);
		DataPtr p(ptr.move(0), len-6);
		switch( id )
		{
			case OGRE_GEOMETRY:
				logMessage("OGRE_GEOMETRY\n");
				readGeometry(p);
				break;
			case OGRE_SUBMESH:
				logMessage("OGRE_SUBMESH\n");
				readSubMesh(p);
				break;
			case OGRE_MESH_BOUNDS:
				logMessage("OGRE_MESH_BOUNDS\n");
				p.move(7*4);
				break;
			case OGRE_SKELETON_LINK: 
				logMessage("OGRE_SKELETON_LINK\n");
				p.move(len-6);
				break;
			case OGRE_BONE_ASSIGNMENT: 
				logMessage("OGRE_BONE_ASSIGNMENT\n");
				p.move(len-6);
				break;
			case OGRE_MESH_LOD: 
				logMessage("OGRE_MESH_LOD\n");
				p.move(len-6);
				break;
			case OGRE_MESH_SUBMESH_NAME_TABLE:
				logMessage("OGRE_MESH_SUBMESH_NAME_TABLE\n");
				p.move(len-6);
				break;
			case OGRE_MESH_EDGE_LISTS:
				logMessage("OGRE_MESH_EDGE_LISTS\n");
				p.move(len-6);
				break;
			default:
				ptr.move(-6);
				logMessage("oops!!!");
				return;
		}
		ptr.move(p.tell());
	}
}

void readSubMesh(DataPtr& ptr)
{
	ushort	id;
	ulong	len, numInd;
	char c;
	ptr.readString(string, 256, '\n');
	ptr.move(1);
	ptr.read(numInd);
	ptr.read(c);
	ptr.move(c?4*numInd:2*numInd);
	while( !ptr.end() )
	{
		ptr.read(id);
		ptr.read(len);
		DataPtr p(ptr.move(0), len-6);
		switch( id )
		{
			case OGRE_GEOMETRY:
				logMessage("OGRE_GEOMETRY\n");
				readGeometry(p);
				break;
			case OGRE_SUBMESH_OPERATION:
				logMessage("OGRE_SUBMESH_OPERATION\n");
				p.move(2);
				break;
			case OGRE_SUBMESH_BONE_ASSIGNMENT:
				logMessage("OGRE_SUBMESH_BONE_ASSIGNMENT\n");
				p.move(len-6);
				break;
			case OGRE_SUBMESH_TEXTURE_ALIAS:
				logMessage("OGRE_SUBMESH_TEXTURE_ALIAS\n");
				p.move(len-6);
			default:
				ptr.move(-6);
				logMessage("oops!!!");
				return;
		}
		ptr.move(p.tell());
	}
}

void readGeometry(DataPtr& ptr)
{
	ushort	id;
	ulong	len;
	ptr.move(4);
	while( !ptr.end() )
	{
		ptr.read(id);
		ptr.read(len);
		DataPtr p(ptr.move(0), len-6);
		switch( id )
		{
			case OGRE_GEOMETRY_VERTEX_DECLARATION:
				logMessage("OGRE_GEOMETRY_VERTEX_DECLARATION\n");
				readGeometryVertexDeclaration(p);
				break;
			case OGRE_GEOMETRY_VERTEX_BUFFER:
				logMessage("OGRE_GEOMETRY_VERTEX_BUFFER\n");
				//readSubMesh(p);
				//break;
			default:
				logMessage("oops!!!");
				p.move(len-6);
		}
		ptr.move(p.tell());
	}
}

void readGeometryVertexDeclaration(DataPtr& ptr)
{
	ushort	id;
	ulong	len;
	while( !ptr.end() )
	{
		ptr.read(id);
		ptr.read(len);
		DataPtr p(ptr.move(0), len);
		switch( id )
		{
			case OGRE_GEOMETRY_VERTEX_ELEMENT:
				logMessage("OGRE_GEOMETRY_VERTEX_ELEMENT\n");
				p.move(10);
				break;
			default:
				logMessage("oops!!!");
				p.move(len-6);
		}
		ptr.move(p.tell());
	}
}
