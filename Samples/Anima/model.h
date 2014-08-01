#pragma once;

#include <core/core.h>
#include <opengl.h>

namespace Model
{
    struct mesh_t;
    struct material_t;
    
    struct model_t
    {
        int            numMeshes;
        mesh_t*        meshes;
        material_t*    materials;
    };

    struct skeleton_t
    {
        int            numJoints;
        int*           boneHierarchy;
        ml::dual_quat* bindPose;
        ml::dual_quat* invBindPose;
    };
    
    struct animation_t
    {
        int            numFrames;
        int            frameRate;
        ml::dual_quat* framePoses;
    };

    struct pose_t
    {
        float          animTime;          // Animation time converted to frames
        ml::dual_quat* boneTransforms;
        ml::dual_quat* pose;              // Current pose, atm used to visualize skeleton
    };

    void init();
    void fini();

    bool loadModel    (const char* name, model_t*     model, skeleton_t* skel);
    bool loadAnimation(const char* name, animation_t* anim,  skeleton_t* skel);

    void createPose(pose_t* pose, skeleton_t* skel);

    void destroyModel    (model_t*     model);
    void destroySkeleton (skeleton_t*  skel );
    void destroyAnimation(animation_t* anim );
    void destroyPose     (pose_t*      pose );

    void update(float fDeltaTime, animation_t* anim, skeleton_t* skel, pose_t* pose);
    void render(model_t* model, skeleton_t* skel, pose_t* pose);
};