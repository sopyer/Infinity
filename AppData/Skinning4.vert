#version 330

#ifndef MAX_BONES
#define MAX_BONES 128
#endif 

#define UNI_GLOBAL  0
#define UNI_BONES   1

layout(std140, column_major) uniform;

layout(location=0) in vec3  aVertex;
layout(location=1) in vec3  aNormal;
layout(location=2) in vec2  aTexCoord0;
layout(location=3) in ivec4 aBoneIndices;
layout(location=4) in vec4  aBoneWeights;

layout(binding = UNI_GLOBAL) uniform uniGlobal
{
    mat4 u_MVP;
};

layout(binding = UNI_BONES) uniform uniBones
{
    vec4 uBoneDualQuat[2*MAX_BONES];
};

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoord0;


void main()
{
    float finalWeight = 1.0f - ( aBoneWeights.x + aBoneWeights.y + aBoneWeights.z );

    mat2x4 dq0 = mat2x4(uBoneDualQuat[2*aBoneIndices.x], uBoneDualQuat[2*aBoneIndices.x+1]);
    mat2x4 dq1 = mat2x4(uBoneDualQuat[2*aBoneIndices.y], uBoneDualQuat[2*aBoneIndices.y+1]);
    mat2x4 dq2 = mat2x4(uBoneDualQuat[2*aBoneIndices.z], uBoneDualQuat[2*aBoneIndices.z+1]);
    mat2x4 dq3 = mat2x4(uBoneDualQuat[2*aBoneIndices.w], uBoneDualQuat[2*aBoneIndices.w+1]);

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

    vec3 position = aVertex + 2.0*cross(blendDQ[0].xyz, cross(blendDQ[0].xyz, aVertex) + blendDQ[0].w*aVertex);
    vec3 normal   = aNormal + 2.0*cross(blendDQ[0].xyz, cross(blendDQ[0].xyz, aNormal) + blendDQ[0].w*aNormal);
    vec3 trans    = 2.0*(blendDQ[0].w*blendDQ[1].xyz - blendDQ[1].w*blendDQ[0].xyz + cross(blendDQ[0].xyz, blendDQ[1].xyz));
    position += trans;

    gl_Position = u_MVP * vec4(position, 1.0);
    vPosition   = position;
    vNormal     = normal;
    vTexCoord0  = aTexCoord0;
}