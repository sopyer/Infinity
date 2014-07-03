#if !defined(FRAMEWORK_CAMERA_H)
#define FRAMEWORK_CAMERA_H

#include <glm/glm.h>
#include <glm/glmext.h>
#include <ml.h>

class SpectatorCamera
{
public:
    SpectatorCamera();

    void lookAt(v128 eye, v128 dir);

    void rotateSmoothly(float heading, float pitch);
    void updatePosition(const glm::vec3 &direction, float elapsedTimeSec);

    void getViewMatrix(v128* view);

    const v128 getPosition()    const { return mPos; } 
    const v128 getOrientation() const { return mOrient; }

    void setOrientation(v128 newOrient);
    void setPosition(v128 newEye);

    glm::vec3 maxVelocity;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    float     rotationSpeed;

private:
    float mAccumPitch;

    v128 mPos;
    v128 mOrient;
};

#endif