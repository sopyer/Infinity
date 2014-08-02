#if !defined(FRAMEWORK_CAMERA_H)
#define FRAMEWORK_CAMERA_H

#include <core/ml.h>

class SpectatorCamera
{
public:
    SpectatorCamera();

    void lookAt(v128 eye, v128 dir);

    void rotateSmoothly(float heading, float pitch);
    void updatePosition(float dx, float dy, float dz, float dt);

    void getViewMatrix(v128* view);

    const v128 getPosition()    const { return mPos; } 
    const v128 getOrientation() const { return mOrient; }

    void setOrientation(v128 newOrient);
    void setPosition(v128 newEye);

    ml::vec3 maxVelocity;
    ml::vec3 acceleration;
    float    rotationSpeed;

private:
    float mAccumPitch;

    v128 mPos;
    v128 mOrient;
    v128 mVelocity;
};

#endif