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

#include "SpectatorCamera.h"
#include <math.h>
#include <ml.h>

const float SpectatorCamera::DEFAULT_ROTATION_SPEED = 0.3f;

const glm::vec3 SpectatorCamera::WORLD_XAXIS(1.0f, 0.0f, 0.0f);
const glm::vec3 SpectatorCamera::WORLD_YAXIS(0.0f, 1.0f, 0.0f);
const glm::vec3 SpectatorCamera::WORLD_ZAXIS(0.0f, 0.0f, 1.0f);

SpectatorCamera::SpectatorCamera()
{
    m_accumPitchDegrees = 0.0f;
    
    rotationSpeed = DEFAULT_ROTATION_SPEED;
}

SpectatorCamera::~SpectatorCamera()
{
}

void SpectatorCamera::lookAt(const glm::vec3 &eye, const glm::vec3 &target, const glm::vec3& up)
{
    m_eye = eye;

    glm::vec3 zAxis = eye - target;
    zAxis = glm::normalize(zAxis);

    glm::vec3 xAxis = glm::cross(up, zAxis);
    xAxis = normalize(xAxis);

    glm::vec3 yAxis = glm::cross(zAxis, xAxis);
    yAxis= normalize(yAxis);
    
    glm::mat4 viewMatrix;

    viewMatrix[0][0] = xAxis.x;
    viewMatrix[1][0] = xAxis.y;
    viewMatrix[2][0] = xAxis.z;
    viewMatrix[3][0] = -glm::dot(xAxis, eye);

    viewMatrix[0][1] = yAxis.x;
    viewMatrix[1][1] = yAxis.y;
    viewMatrix[2][1] = yAxis.z;
    viewMatrix[3][1] = -glm::dot(yAxis, eye);

    viewMatrix[0][2] = zAxis.x;
    viewMatrix[1][2] = zAxis.y;
    viewMatrix[2][2] = zAxis.z;    
    viewMatrix[3][2] = -glm::dot(zAxis, eye);

    // Extract the pitch angle from the view matrix.
    m_accumPitchDegrees = glm::degrees(asinf(viewMatrix[1][2]));
    
    m_orientation = glm::quatGTX(viewMatrix);
}

void SpectatorCamera::move(float dx, float dy, float dz)
{
    // Moves the camera by dx world units to the left or right; dy
    // world units upwards or downwards; and dz world units forwards
    // or backwards.

    glm::mat4 viewMatrix = glm::mat4GTX(m_orientation);

    glm::vec3 eye = m_eye;
    glm::vec3 xAxis(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
    glm::vec3 forwards(-viewMatrix[0][2], -viewMatrix[1][2], -viewMatrix[2][2]);

    eye += xAxis * dx;
    eye += WORLD_YAXIS * dy;
    eye += forwards * dz;

    setPosition(eye);
}

void SpectatorCamera::rotateSmoothly(float headingDegrees, float pitchDegrees)
{
    // This method applies a scaling factor to the rotation angles prior to
    // using these rotation angles to rotate the camera. This method is usually
    // called when the camera is being rotated using an input device (such as a
    // mouse or a joystick). 

    headingDegrees *= -rotationSpeed;
    pitchDegrees   *= -rotationSpeed;

    // Implements the rotation logic for the first person style and
    // spectator style camera behaviors. Roll is ignored.

    m_accumPitchDegrees += pitchDegrees;

    if (m_accumPitchDegrees > 90.0f)
    {
        pitchDegrees = 90.0f - (m_accumPitchDegrees - pitchDegrees);
        m_accumPitchDegrees = 90.0f;
    }

    if (m_accumPitchDegrees < -90.0f)
    {
        pitchDegrees = -90.0f - (m_accumPitchDegrees - pitchDegrees);
        m_accumPitchDegrees = -90.0f;
    }
   
    glm::__quatGTX rot;

    // Rotate camera about the world y axis.
    // Note the order the quaternions are multiplied. That is important!
    if (headingDegrees != 0.0f)
    {
        rot = glm::angleAxisGTX(headingDegrees, WORLD_YAXIS);
        //m_orientation = rot * m_orientation;
        m_orientation = glm::crossGTX(m_orientation, rot);
    }

    // Rotate camera about its local x axis.
    // Note the order the quaternions are multiplied. That is important!
    if (pitchDegrees != 0.0f)
    {
        rot = glm::angleAxisGTX(pitchDegrees, WORLD_XAXIS);
        //m_orientation = m_orientation * rot;
        m_orientation = glm::crossGTX(rot, m_orientation);
    }
}

void SpectatorCamera::updatePosition(const glm::vec3 &direction, float elapsedTimeSec)
{
    // Moves the camera using Newton's second law of motion. Unit mass is
    // assumed here to somewhat simplify the calculations. The direction vector
    // is in the range [-1,1].

    if (glm::length(velocity) != 0.0f)
    {
        // Only move the camera if the velocity vector is not of zero length.
        // Doing this guards against the camera slowly creeping around due to
        // floating point rounding errors.

        glm::vec3 displacement = (velocity * elapsedTimeSec) +
            (0.5f * acceleration * elapsedTimeSec * elapsedTimeSec);

        // Floating point rounding errors will slowly accumulate and cause the
        // camera to move along each axis. To prevent any unintended movement
        // the displacement vector is clamped to zero for each direction that
        // the camera isn't moving in. Note that the updateVelocity() method
        // will slowly decelerate the camera's velocity back to a stationary
        // state when the camera is no longer moving along that direction. To
        // account for this the camera's current velocity is also checked.

        if (direction.x == 0.0f && glm::equalEpsilonGTX(velocity.x, 0.0f, 1e-6f))
            displacement.x = 0.0f;

        if (direction.y == 0.0f && glm::equalEpsilonGTX(velocity.y, 0.0f, 1e-6f))
            displacement.y = 0.0f;

        if (direction.z == 0.0f && glm::equalEpsilonGTX(velocity.z, 0.0f, 1e-6f))
            displacement.z = 0.0f;

        move(displacement.x, displacement.y, displacement.z);
    }

    // Continuously update the camera's velocity vector even if the camera
    // hasn't moved during this call. When the camera is no longer being moved
    // the camera is decelerating back to its stationary state.

    updateVelocity(direction, elapsedTimeSec);
}

void SpectatorCamera::setOrientation(const glm::__quatGTX& newOrientation)
{
    glm::mat4 m = glm::mat4GTX(newOrientation);

    // Store the pitch for this new orientation.
    // First person and spectator behaviors limit pitching to
    // 90 degrees straight up and down.

    m_accumPitchDegrees = glm::degrees(asinf(m[1][2]));

    m_orientation       = newOrientation;

    // First person and spectator behaviors don't allow rolling.
    // Negate any rolling that might be encoded in the new orientation.
    glm::vec3 forwards(-m[0][2], -m[1][2], -m[2][2]);
    lookAt(m_eye, m_eye + forwards);

}

void SpectatorCamera::setPosition(const glm::vec3 &newEye)
{
    m_eye = newEye;
}

void SpectatorCamera::updateVelocity(const glm::vec3 &direction, float elapsedTimeSec)
{
    // Updates the camera's velocity based on the supplied movement direction
    // and the elapsed time (since this method was last called). The movement
    // direction is in the range [-1,1].

    if (direction.x != 0.0f)
    {
        // SpectatorCamera is moving along the x axis.
        // Linearly accelerate up to the camera's max speed.

        velocity.x += direction.x * acceleration.x * elapsedTimeSec;

        if (velocity.x > maxVelocity.x)
            velocity.x = maxVelocity.x;
        else if (velocity.x < -maxVelocity.x)
            velocity.x = -maxVelocity.x;
    }
    else
    {
        // SpectatorCamera is no longer moving along the x axis.
        // Linearly decelerate back to stationary state.

        if (velocity.x > 0.0f)
        {
            if ((velocity.x -= acceleration.x * elapsedTimeSec) < 0.0f)
                velocity.x = 0.0f;
        }
        else
        {
            if ((velocity.x += acceleration.x * elapsedTimeSec) > 0.0f)
                velocity.x = 0.0f;
        }
    }

    if (direction.y != 0.0f)
    {
        // SpectatorCamera is moving along the y axis.
        // Linearly accelerate up to the camera's max speed.

        velocity.y += direction.y * acceleration.y * elapsedTimeSec;
        
        if (velocity.y > maxVelocity.y)
            velocity.y = maxVelocity.y;
        else if (velocity.y < -maxVelocity.y)
            velocity.y = -maxVelocity.y;
    }
    else
    {
        // SpectatorCamera is no longer moving along the y axis.
        // Linearly decelerate back to stationary state.

        if (velocity.y > 0.0f)
        {
            if ((velocity.y -= acceleration.y * elapsedTimeSec) < 0.0f)
                velocity.y = 0.0f;
        }
        else
        {
            if ((velocity.y += acceleration.y * elapsedTimeSec) > 0.0f)
                velocity.y = 0.0f;
        }
    }

    if (direction.z != 0.0f)
    {
        // SpectatorCamera is moving along the z axis.
        // Linearly accelerate up to the camera's max speed.

        velocity.z += direction.z * acceleration.z * elapsedTimeSec;

        if (velocity.z > maxVelocity.z)
            velocity.z = maxVelocity.z;
        else if (velocity.z < -maxVelocity.z)
            velocity.z = -maxVelocity.z;
    }
    else
    {
        // SpectatorCamera is no longer moving along the z axis.
        // Linearly decelerate back to stationary state.

        if (velocity.z > 0.0f)
        {
            if ((velocity.z -= acceleration.z * elapsedTimeSec) < 0.0f)
                velocity.z = 0.0f;
        }
        else
        {
            if ((velocity.z += acceleration.z * elapsedTimeSec) > 0.0f)
                velocity.z = 0.0f;
        }
    }
}

void SpectatorCamera::getViewMatrix(glm::mat4& view)
{
    // Reconstruct the view matrix.

    v128 v[4];
    v128 q = vi_loadu_v4((ml::quat*)&m_orientation.x);
    ml::quat_to_mat4(v, ml::conjugate_quat(q));
    glm::mat4* m = (glm::mat4*)v;

    view = glm::mat4GTX(m_orientation);

    glm::vec3 xAxis(view[0][0], view[1][0], view[2][0]);
    glm::vec3 yAxis(view[0][1], view[1][1], view[2][1]);
    glm::vec3 zAxis(view[0][2], view[1][2], view[2][2]);

    v128 eye = vi_loadu_v4((ml::vec4*)(float*)glm::vec4(m_eye, 1.0f));

    v128 t = ml::conjugate_quat(ml::rotate_vec3_quat(q, eye));

    view[3][0] = -glm::dot(xAxis, m_eye);
    view[3][1] = -glm::dot(yAxis, m_eye);
    view[3][2] = -glm::dot(zAxis, m_eye);
}

void SpectatorCamera::getViewMatrixSSE(v128* mat)
{
    // Reconstruct the view matrix.
    v128 q   = vi_loadu_v4((ml::quat*)&m_orientation.x);
    v128 eye = vi_loadu_v4((ml::vec4*)(float*)glm::vec4(m_eye, 1.0f));

    ml::quat_to_mat4x3(mat, q);
    mat[3] = ml::conjugate_quat(ml::rotate_vec3_quat(q, eye));

}
