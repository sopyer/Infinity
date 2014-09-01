#version 430

#ifndef MAX_BONES
#define MAX_BONES 128
#endif 

#define UNI_GLOBAL  0
#define UNI_BONES   1

layout(std140, column_major) uniform;

layout(location=0) in vec3  aVertex;
layout(location=3) in ivec4 aBoneIndices;
layout(location=4) in vec4  aBoneWeights;

layout(binding = UNI_GLOBAL) uniform uniGlobal
{
    mat4 au_MV;
    vec4 au_Proj;
};

layout(std140, binding = UNI_BONES) buffer uniBones
{
    struct dual_quat_t
    {
        vec4 real, dual;
    } uBones[];
};

out vec3 gaPosition;

void main()
{
    float finalWeight = 1.0f - ( aBoneWeights.x + aBoneWeights.y + aBoneWeights.z );

    mat2x4 dq0 = mat2x4(uBones[aBoneIndices.x].real, uBones[aBoneIndices.x].dual);
    mat2x4 dq1 = mat2x4(uBones[aBoneIndices.y].real, uBones[aBoneIndices.y].dual);
    mat2x4 dq2 = mat2x4(uBones[aBoneIndices.z].real, uBones[aBoneIndices.z].dual);
    mat2x4 dq3 = mat2x4(uBones[aBoneIndices.w].real, uBones[aBoneIndices.w].dual);

    if (dot(dq0[0], dq1[0]) < 0.0) dq1 *= -1.0;
    if (dot(dq0[0], dq2[0]) < 0.0) dq2 *= -1.0;
    if (dot(dq0[0], dq3[0]) < 0.0) dq3 *= -1.0;

    mat2x4 blendDQ;
    blendDQ  = aBoneWeights.x * dq0;
    blendDQ += aBoneWeights.y * dq1;
    blendDQ += aBoneWeights.z * dq2;
    blendDQ += finalWeight    * dq3;

    float len = length(blendDQ[0]);
    blendDQ /= len;

    vec3 p = aVertex + 2.0*cross(blendDQ[0].xyz, cross(blendDQ[0].xyz, aVertex) + blendDQ[0].w*aVertex);
    p += 2.0*(blendDQ[0].w*blendDQ[1].xyz - blendDQ[1].w*blendDQ[0].xyz + cross(blendDQ[0].xyz, blendDQ[1].xyz));

    vec4 viewPos = au_MV * vec4(p, 1.0);

    gl_Position = vec4(viewPos.xy * au_Proj.xy, au_Proj.z*viewPos.z+au_Proj.w, -viewPos.z); //projection
    gaPosition  = p;
}