#pragma once;

#include <ml.h>
#include <opengl.h>

struct Mesh;

class MD5Model
{
public:
	MD5Model();
    ~MD5Model();

    bool LoadModel(const char* name);
    bool LoadAnim (const char *name);

    void Update(float fDeltaTime);
    void Render(float* MVP);

	void resetUniforms();

public:
   	GLuint program;

private:
    void RenderMesh    (Mesh& mesh);

private:
    bool                mHasAnimation;

    // mesh data
    int                 mNumMeshes;
    Mesh*               mMeshes;
    
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
	GLint uTexture;
	GLint uBoneMatrix;
    GLint uBoneDualQuat;
	GLint uMVP;
};