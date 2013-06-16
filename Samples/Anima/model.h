#pragma once;

#include <ml.h>
#include <opengl.h>

struct Mesh;
struct Material;

class MD5Model
{
public:
	MD5Model();
    ~MD5Model();

    bool LoadModel(const char* name);
    bool LoadAnim (const char *name);

    void Update(float fDeltaTime);
    void Render(float* MVP, v128 shPoly[9], float* matSHRed, float* matSHGreen, float* matSHBlue);

public:
   	GLuint prgLighting;
    GLuint prgDefault;

private:
    void RenderMesh    (Mesh* mesh, Material* material);

private:
    bool                mHasAnimation;

    // mesh data
    int                 mNumMeshes;
    Mesh*               mMeshes;
    Material*           mMaterials;
    Material*           mWireframe;

    // skeleton data
    int                 mNumJoints;
    int*                mBoneHierarchy;
    ml::dual_quat*      mBindPose;
    ml::dual_quat*      mInvBindPose;

    // animetion data
    int                 mNumFrames;
    int                 mFrameRate;
    ml::dual_quat*      mFramePoses;
    
    // working set
    float               mAnimTime;          // Animation time converted to frames
    ml::dual_quat*      mBoneTransform;
    ml::dual_quat*      mPose;              // Current pose, atm used to visualize skeleton

    // uniform locations
    GLuint   ubo;
    GLsizei  uboSize;
    GLsizei  uniGlobal;
    GLsizei  uniBones;
    GLsizei  uniLighting;
};