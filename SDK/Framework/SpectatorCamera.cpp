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

const float DEFAULT_ROTATION_SPEED = 0.3f;

SpectatorCamera::SpectatorCamera()
{
    mPos          = vi_set_0001f();
    mOrient       = vi_set_0001f();
    mVelocity     = vi_set_0000();
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
    mAccumPitch = ml::asin(m[2].m128_f32[1]);

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

void SpectatorCamera::updatePosition(float dx, float dy, float dz, float dt)
{
    // Continuously update the camera's velocity vector even if the camera
    // hasn't moved during this call. When the camera is no longer being moved
    // the camera is decelerating back to its stationary state.

    // Updates the camera's velocity based on the supplied movement direction
    // and the elapsed time (since this method was last called). The movement
    // direction is in the range [-1,1].

    v128 dir, delta, vp, mv, dv, a, mask;

    dir = vi_set(dx, dy, -dz, 0.0f);
    vp  = mVelocity;
    mv  = vi_load_v3(&maxVelocity);
    a   = vi_load_v3(&acceleration);

    mask = vi_cmp_eq(dir, vi_set_0000());

    // choose direction for deacceleration if corresponding delta==0
    dir = vi_select(dir, vi_neg(vi_sign(vp)), mask);

    // calculate new velocity v = dir * a * dt + pv
    dv        = vi_mul(vi_set_fff0(dt), a);
    mVelocity = vi_mad(dir, dv, vp);

    mask = vi_and(mask, vi_cmp_lt(vi_abs(vp), dv));

    // in order to stop movement when deaccelerating
    // make zero all coefficients accordingly to mask
    mVelocity = vi_andnot(mVelocity, mask);
    // and clip result to [-maxVelocity, maxVelocity]
    // in order to not exceed maximum velocity
    mVelocity = vi_clamp(mVelocity, vi_neg(mv), mv);

    // integrate distance using trapezoid rule(midpoint integration)
    delta = vi_mul(vi_set_fff0(0.5f * dt), vi_add(vp, mVelocity));

    mPos = vi_add(
        ml::rotate_vec3_quat(
            ml::conjugate_quat(mOrient),
            vi_swizzle<VI_SWIZZLE_MASK(0, 3, 2, 3)>(delta)
        ),
        mPos
    );
    mPos.m128_f32[1] += delta.m128_f32[1];
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
