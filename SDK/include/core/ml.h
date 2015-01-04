#pragma once

#include <stdint.h>
#include <limits.h>
#include <float.h>

#include <core/vi.h>

#define FLT_PI               3.141592653589793f
#define FLT_2_PI             6.283185307179586f
#define FLT_PI_OVER_2        1.570796326794897f
#define FLT_1_OVER_PI        0.318309886183791f
#define FLT_1_OVER_2_PI      0.159154943091895f
#define FLT_RAD_TO_DEG_SCALE 57.29577951308232f
#define FLT_DEG_TO_RAD_SCALE 0.017453292519943f
#define ML_C_EPS3            0.001f
#define ML_C_EPS5            0.00001f
#define ML_C_EPS7            0.0000001f

namespace ml
{
    enum
    {
        IT_INSIDE,
        IT_OUTSIDE,
        IT_INTERSECT
    };

    struct vec2
    {
        float x, y;
    };

    struct vec3
    {
        float x, y, z;
    };

    struct vec4
    {
        float x, y, z, w;
    };

    struct quat
    {
        float x, y, z, w;
    };

    struct dual_quat
    {
        quat real;
        quat dual;
    };

    struct mat4x4
    {
        v128 r0;
        v128 r1;
        v128 r2;
        v128 r3;
    };

    struct aabb
    {
        v128 min, max;
    };

    // quaternions
    void mul_quat        (quat* result, quat* a, quat* b); // multiplies 2 quaternions, returns pointer to result
    void conjugate_quat  (quat* result, quat* q);
    void rotate_vec3_quat(vec3* result, quat* q, vec3* p);

    //v128 version of quaternion operations
    v128 mul_quat             (v128 q0, v128 q1);
    v128 rotate_vec3_quat     (v128 q, v128 v);
    v128 conjugate_quat       (v128 q);
    v128 lerp_quat            (v128 q0, v128 q1, float t);
    v128 translation_dual_quat(v128 rq, v128 dq);
    v128 length_quat          (v128 q);
    v128 normalize_quat       (v128 q);

    v128 normalize            (v128 v);
    v128 length               (v128 v);

    v128 make_p3(v128 v);   // set v.w = 1.0f
    v128 make_v3(v128 v);   // set v.w = 0.0f

    // dual quaternions
    void set_identity_dual_quat        (dual_quat* dq);
    void make_dual_quat                (dual_quat* result, quat* orient, vec3* offset);
    void mul_dual_quat                 (dual_quat* result, dual_quat* dq0, dual_quat* dq1);
    void conjugate_dual_quat           (dual_quat* result, dual_quat* dq);
    void transform_vec3_dual_quat      (vec3* result, dual_quat* dq, vec3* p);
    void get_translation_from_dual_quat(vec3* result, dual_quat* dq);
    v128 transform_vec3_dual_quat      (dual_quat* dq, v128 p);
    void make_translation_mat4         (v128* res, float dx, float dy, float dz);
    void make_rotation_mat4            (v128* res, float angle, float axisx, float axisy, float axisz);

    // matrices
    v128 mul_mat4_vec4     (v128* m/*[4]*/, v128 v);
    void transpose_mat4    (v128* r/*[4]*/, v128* m/*[4]*/);
    void transpose_mat4x2  (v128* r/*[2]*/, v128* m/*[4]*/);
    void mul_mat4          (v128* r/*[4]*/, v128* a/*[4]*/, v128* b/*[4]*/);
    void quat_to_mat4x3    (v128* m/*[3]*/, v128 q);
    void quat_to_mat4      (v128* m/*[4]*/, v128 q);
    void quat_vec3_to_mat4 (v128* m/*[4]*/, v128 quat, v128 pos);
    v128 mat4x3_to_quat    (v128* m/*[4]*/);
    v128 axis_angle_to_quat(v128 axis, float angle);

    void make_perspective_mat4    (v128* mat/*[4]*/, float fovy, float aspect, float zn, float zf);
    void make_inf_perspective_mat4(v128* mat/*[4]*/, float fovy, float aspect, float zn);
    void make_ortho2D_mat4        (v128* mat/*[4]*/, float left, float right, float bottom, float top);
    void make_ortho3D_mat4        (v128* mat/*[4]*/, float left, float right, float bottom, float top, float zn, float zf);
    void make_identity_mat4       (v128* mat/*[4]*/);

    v128 distanceToAABB(v128 vmin, v128 vmax, v128 pt);
    bool sphereAABBTest2(aabb* AABB, v128 center, float radius, v128 res);
    bool sphereAABBTest(aabb* AABB, v128 center, float radius);
    void transformPointsSOA4(v128* dest, v128 xxxx, v128 yyyy, v128 zzzz, /*vec4 wwww,*/ const mat4x4* matrix);
    int  intersectionTest(const aabb* bbox, const mat4x4* frustum);

}

namespace ml
{
    /*****************************************/
    /**************   scalar   ***************/
    /*****************************************/

    template<typename T>
    inline T min(T x, T y)
    {
        return (x < y) ? x : y;
    }

    template<typename T>
    inline T max(T x, T y)
    {
        return (x > y) ? x : y;
    }

    template<typename T>
    inline T clamp(T val, T minVal, T maxVal)
    {
        return max(minVal, min(val, maxVal));
    }

    /*****************************************/
    /****************   vec2   ***************/
    /*****************************************/
    inline vec2 operator-(const vec2& arg)
    {
        return { -arg.x, -arg.y };
    }

    inline vec2 operator+(const vec2& arg0, const vec2& arg1)
    {
        return { arg0.x + arg1.x, arg0.y + arg1.y };
    }

    inline vec2 operator-(const vec2& arg0, const vec2& arg1)
    {
        return { arg0.x - arg1.x, arg0.y - arg1.y };
    }

    inline vec2 operator*(float arg0, const vec2& arg1)
    {
        return { arg0*arg1.x, arg0*arg1.y };
    }

    inline vec2 operator*(const vec2& arg0, const vec2& arg1)
    {
        return { arg0.x*arg1.x, arg0.y*arg1.y };
    }

    inline vec2 operator/(const vec2& arg0, const vec2& arg1)
    {
        return { arg0.x / arg1.x, arg0.y / arg1.y };
    }


    /*****************************************/
    /****************   vec3   ***************/
    /*****************************************/
    inline vec3 operator-(const vec3& arg)
    {
        return { -arg.x, -arg.y, -arg.z };
    }

    inline vec3 operator+(const vec3& arg0, const vec3& arg1)
    {
        return { arg0.x+arg1.x, arg0.y+arg1.y, arg0.z+arg1.z };
    }

    inline vec3 operator-(const vec3& arg0, const vec3& arg1)
    {
        return { arg0.x-arg1.x, arg0.y-arg1.y, arg0.z-arg1.z };
    }

    inline vec3 operator*(float arg0, const vec3& arg1)
    {
        return { arg0*arg1.x, arg0*arg1.y, arg0*arg1.z };
    }

    inline vec3 operator*(const vec3& arg0, const vec3& arg1)
    {
        return { arg0.x*arg1.x, arg0.y*arg1.y, arg0.z*arg1.z };
    }

    inline vec3 operator/(const vec3& arg0, const vec3& arg1)
    {
        return { arg0.x / arg1.x, arg0.y / arg1.y, arg0.z / arg1.z };
    }

    inline vec3 min(const vec3& arg0, const vec3& arg1)
    {
        return {
            arg0.x < arg1.x ? arg0.x : arg1.x,
            arg0.y < arg1.y ? arg0.y : arg1.y,
            arg0.z < arg1.z ? arg0.z : arg1.z
        };
    }

    inline vec3 max(const vec3& arg0, const vec3& arg1)
    {
        return{
            arg0.x > arg1.x ? arg0.x : arg1.x,
            arg0.y > arg1.y ? arg0.y : arg1.y,
            arg0.z > arg1.z ? arg0.z : arg1.z
        };
    }


    /*****************************************/
    /****************   vec4   ***************/
    /*****************************************/
    inline vec4 operator-(const vec4& arg)
    {
        return { -arg.x, -arg.y, -arg.z, -arg.w };
    }

    inline vec4 operator+(const vec4& arg0, const vec4& arg1)
    {
        return { arg0.x + arg1.x, arg0.y + arg1.y, arg0.z + arg1.z, arg0.w + arg1.w };
    }

    inline vec4 operator-(const vec4& arg0, const vec4& arg1)
    {
        return { arg0.x - arg1.x, arg0.y - arg1.y, arg0.z - arg1.z, arg0.w - arg1.w };
    }

    inline vec4 operator*(float arg0, const vec4& arg1)
    {
        return { arg0*arg1.x, arg0*arg1.y, arg0*arg1.z, arg0*arg1.w };
    }

    inline vec4 operator*(const vec4& arg0, const vec4& arg1)
    {
        return { arg0.x*arg1.x, arg0.y*arg1.y, arg0.z*arg1.z, arg0.w*arg1.w };
    }

    inline vec4 operator/(const vec4& arg0, const vec4& arg1)
    {
        return { arg0.x / arg1.x, arg0.y / arg1.y, arg0.z / arg1.z, arg0.w / arg1.w };
    }
}

// Scalar math functions
namespace ml
{
    int32_t asint(float f);
    float   asfloat(int32_t i);

    float pow  (float x, float y);
    float exp  (float x);
    float ln   (float x);
    float lg2  (float x);
    float lg10 (float x);
    float sqrt (float x);
    float sin  (float x);
    float cos  (float x);
    float tan  (float x);
    float asin (float x);
    float atan2(float y, float x);

    float abs  (float x);
    float floor(float x);
    float ceil (float x);

    float mod(float x, float y);
}
