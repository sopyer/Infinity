#pragma once;

#include <ml.h>
#include <opengl.h>
#include <vector>

struct Skinning4PointShader
{
	GLuint program;

	GLint uTexture;
	GLint uBoneMatrix;
    GLint uBoneDualQuat;
	GLint uMVP;

	void Create();
	void bindUniforms();
	void Destroy();
};

struct Mesh
{
	GLuint			vbo;
	GLuint          ibo;
	GLuint          texture;
	GLsizei			numIndices;
};

typedef std::vector<Mesh>           MeshList;
typedef std::vector<int>            Hierarchy;
typedef std::vector<ml::dual_quat>  DualQuatList;
typedef std::vector<DualQuatList>   AnimPosesList;
typedef std::vector<std::string>    StringList;

class MD5Model
{
public:
    // Resources
	Skinning4PointShader skin4PointShader;

public:
	MD5Model();
    ~MD5Model();

    bool LoadModel( const char* name );
    bool LoadAnim ( const char *name );

    void Update( float fDeltaTime );
    void Render(float* MVP);

protected:
    void RenderMesh    ( Mesh& mesh );
    void RenderSkeleton( DualQuatList& joints );

private:
    bool                mHasAnimation;

    // ------------------
    // --- Model data ---
    // ------------------
    int                 mNumMeshes;
    MeshList            mMeshes;
    
    // Skeleton data
    int                 mNumJoints;
    Hierarchy           mBoneHierarchy;
    
    // Precalculated data used for animation
    DualQuatList        mBindPoseDQ;
    DualQuatList        mInvBindPoseDQ;

    // ----------------------
    // --- Animation data ---
    // ----------------------
    int                 mNumFrames;
    int                 mFrameRate;
    AnimPosesList       mFramePoses;        // Skeleton animation frames
    
    // -------------------
    // --- Working set ---
    // -------------------
    float               mAnimTime;          // Animation time converted to frames
    DualQuatList        mBoneTransformDQ;
    DualQuatList        mPoseDQ;            // Current pose, atm used to visualize skeleton
};