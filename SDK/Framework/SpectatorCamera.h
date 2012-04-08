//-----------------------------------------------------------------------------
// Copyright (c) 2007-2008 dhpoware. All Rights Reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#if !defined(FRAMEWORK_CAMERA_H)
#define FRAMEWORK_CAMERA_H

#include <glm/glm.h>
#include <glm/glmext.h>

//-----------------------------------------------------------------------------
// A general purpose 6DoF (six degrees of freedom) quaternion based camera.
//
// This camera class supports 4 different behaviors:
// first person mode, spectator mode, flight mode, and orbit mode.
//
// First person mode only allows 5DOF (x axis movement, y axis movement, z axis
// movement, yaw, and pitch) and movement is always parallel to the world x-z
// (ground) plane.
//
// Spectator mode is similar to first person mode only movement is along the
// direction the camera is pointing.
// 
// Flight mode supports 6DoF. This is the camera class' default behavior.
//
// Orbit mode rotates the camera around a target position. This mode can be
// used to simulate a third person camera. Orbit mode supports 2 modes of
// operation: orbiting about the target's Y axis, and free orbiting. The former
// mode only allows pitch and yaw. All yaw changes are relative to the target's
// local Y axis. This means that camera yaw changes won't be affected by any
// rolling. The latter mode allows the camera to freely orbit the target. The
// camera is free to pitch, yaw, and roll. All yaw changes are relative to the
// camera's orientation (in space orbiting the target).
//
// This camera class allows the camera to be moved in 2 ways: using fixed
// step world units, and using a supplied velocity and acceleration. The former
// simply moves the camera by the specified amount. To move the camera in this
// way call one of the move() methods. The other way to move the camera
// calculates the camera's displacement based on the supplied velocity,
// acceleration, and elapsed time. To move the camera in this way call the
// updatePosition() method.
//-----------------------------------------------------------------------------

class SpectatorCamera
{
public:
    SpectatorCamera();
    ~SpectatorCamera();

    void lookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up = WORLD_YAXIS);

    void rotateSmoothly(float headingDegrees, float pitchDegrees);
    void updatePosition(const glm::vec3 &direction, float elapsedTimeSec);

    void getViewMatrix(glm::mat4& view);

    const glm::vec3&      getPosition() const { return m_eye; } 
    const glm::__quatGTX& getOrientation() const { return m_orientation; }

    void setOrientation(const glm::__quatGTX& newOrientation);
    void setPosition(const glm::vec3& newEye);

    glm::vec3 maxVelocity;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    float     rotationSpeed;

private:
    void move(float dx, float dy, float dz);
    void updateVelocity(const glm::vec3 &direction, float elapsedTimeSec);

    static const float DEFAULT_ROTATION_SPEED;

    static const glm::vec3 WORLD_XAXIS;
    static const glm::vec3 WORLD_YAXIS;
    static const glm::vec3 WORLD_ZAXIS;

    float m_accumPitchDegrees;

    glm::vec3      m_eye;
    glm::__quatGTX m_orientation;
};

#endif