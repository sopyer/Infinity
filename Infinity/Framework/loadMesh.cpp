#include "Framework.h"
#include <DataPtr.h>

#define MESH_INDICES_MAGIC '10im'

VertexDecl SubMesh::decl_[3]=
{
	{USAGE_POSITION, 3, GL_FLOAT, 0, 8*4},
	{USAGE_NORMAL, 3, GL_FLOAT, 3*4, 8*4},
	{USAGE_TEXCOORD0, 2, GL_FLOAT, 6*4, 8*4}
};

MeshPtr loadMesh(const char* path)
{
	int magic;
	MeshPtr mesh;
	DataStreamPtr stream = vfsMapFile(path);
	if(!stream)
		return mesh;
	
	DataPtr ptr(stream->MapStream(), stream->GetSize());

	ptr.read(magic);
	
	if(magic == MESH_INDICES_MAGIC) 
	{
		int num_surfaces;
		ptr.read(num_surfaces);
		mesh = MeshPtr(new Mesh);
		for(int i = 0; i < num_surfaces; i++) {
			SubMesh *s = new SubMesh();
			
			ptr.move(128);
			// name
			//fread(s->name,sizeof(s->name),1,file);
			
			// vertexes
			ptr.read(s->numVert_);
			s->vert_.setData(8*4*s->numVert_, ptr.move(8*4*s->numVert_));
			//mesh_Vertex *vertex = new mesh_Vertex[num_vertex];
			//fread(vertex,sizeof(mesh_Vertex),num_vertex,file);
			
			// triangles
			ptr.read(s->numInd_);
			s->numInd_*=3;
			s->ind_.setData(s->numInd_*4, ptr.move(s->numInd_*4));
			mesh->push_back(SubMeshPtr(s));
		}
	}
	return mesh;
}