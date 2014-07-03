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
#include <ml.h>

const float DEFAULT_ROTATION_SPEED = 0.3f;

SpectatorCamera::SpectatorCamera()
{
    mPos          = vi_set_0001f();
    mOrient       = vi_set_0001f();
    mAccumPitch   = 0.0f;
    rotationSpeed = DEFAULT_ROTATION_SPEED;
}

void SpectatorCamera::lookAt(v128 eye, v128 dir)
{
    mPos = eye;

    v128 xaxis, yaxis, zaxis;

    zaxis = ml::normalize(dir);

    xaxis = vi_cross3(vi_set_0100f(), zaxis);
    xaxis = ml::normalize(xaxis);

    yaxis = vi_cross3(zaxis, xaxis);
    yaxis = ml::normalize(yaxis);

    v128 m[3] = {xaxis, yaxis, zaxis};

    // Extract the pitch angle from the view matrix.
    mAccumPitch = ml::asinf(m[2].m128_f32[1]);

    mOrient = ml::conjugate_quat(ml::mat4x3_to_quat(m));
}

void SpectatorCamera::rotateSmoothly(float heading, float pitch)
{
    v128 r;

    heading *= -rotationSpeed;
    pitch   *= -rotationSpeed;

    mAccumPitch += pitch;

    if (mAccumPitch > FLT_PI_OVER_2)
    {
        pitch = FLT_PI_OVER_2 - (mAccumPitch - pitch);
        mAccumPitch = FLT_PI_OVER_2;
    }

    if (mAccumPitch < -FLT_PI_OVER_2)
    {
        pitch = -FLT_PI_OVER_2 - (mAccumPitch - pitch);
        mAccumPitch = -FLT_PI_OVER_2;
    }

    // Rotate camera first around y axis, then around x axis.
    // Note the order the quaternions are multiplied. That is important!
    r = ml::axis_angle_to_quat(vi_set_0100f(), heading);
    mOrient = ml::mul_quat(mOrient, r);
    r = ml::axis_angle_to_quat(vi_set_1000f(), pitch);
    mOrient = ml::mul_quat(r, mOrient);
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

        v128 mat[3];

        ml::quat_to_mat4x3(mat, ml::conjugate_quat(mOrient));

        mPos = vi_mad(mat[0], vi_set_fff0(displacement.x), mPos);
        mPos = vi_mad(vi_set_0100f(), vi_set_fff0(displacement.y), mPos);
        mPos = vi_mad(mat[2], vi_set_fff0(-displacement.z), mPos);
    }

    // Continuously update the camera's velocity vector even if the camera
    // hasn't moved during this call. When the camera is no longer being moved
    // the camera is decelerating back to its stationary state.

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

void SpectatorCamera::setOrientation(v128 newOrient)
{
    v128 m[3];

    ml::quat_to_mat4x3(m, ml::conjugate_quat(newOrient));

    // First person and spectator behaviors don't allow rolling.
    // Negate any rolling that might be encoded in the new orientation.
    lookAt(mPos, m[2]);
}

void SpectatorCamera::setPosition(v128 newEye)
{
    mPos = newEye;
    mPos.m128_i32[3] = FLT_1_0_ASINT;
}

void SpectatorCamera::getViewMatrix(v128* mat)
{
    ml::quat_to_mat4x3(mat, mOrient);
    mat[3] = ml::conjugate_quat(ml::rotate_vec3_quat(mOrient, mPos));
}
