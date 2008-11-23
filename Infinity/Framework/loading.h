#ifndef _LOADING_H_INCLUDED_
#	define _LOADING_H_INCLUDED_

#include <boost\shared_ptr.hpp>
#include <utils.h>
#include <ResourceManager.h>

typedef boost::shared_ptr<glShaderBase> glShaderPtr;
typedef boost::shared_ptr<glProgram> glProgramPtr;
typedef boost::shared_ptr<glTexture2D> glTexture2DPtr;

glShaderBase* loadShader(const char* path);
glProgram* loadProgram(const char* shaders);
glTexture2D* loadTargaTexture(const char* path);
glTexture2D* loadJpegTexture(const char* path);
glTexture2D* loadPngTexture(const char* path);

class VertexDeclaration: public utils::vector<VertexDecl>
{
	private:
		typedef utils::vector<VertexDecl> base;
	public:
		VertexDeclaration():base(7){}
};

struct SubMesh
{
	long numVert_, numInd_;
	glAttribBuffer	vert_;
	glIndexBuffer	ind_;
	static VertexDecl decl_[3];
};

typedef boost::shared_ptr<SubMesh> SubMeshPtr;
typedef utils::vector<SubMeshPtr> Mesh;
typedef boost::shared_ptr<Mesh> MeshPtr;

MeshPtr loadMesh(const char* path);

#endif