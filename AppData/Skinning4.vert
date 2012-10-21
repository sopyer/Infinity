#version 330

#ifndef MAX_BONES
#define MAX_BONES 128
#endif 

layout(location=0) in vec3  aVertex;
layout(location=1) in vec3  aNormal;
layout(location=2) in vec2  aTexCoord;
layout(location=3) in ivec4 aBoneIndices;
layout(location=4) in vec4  aBoneWeights;

uniform mat4 uMVP;
uniform mat4 uBoneMatrix[MAX_BONES];

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoord0;

//void main()
//{
//	vec4  weights = aBoneWeights;
//	vec4  weightIndices = aBoneIndices;
//    mat4  matTransform;
//    float finalWeight = 1.0f - ( weights.x + weights.y + weights.z );
//
//	matTransform  = uBoneMatrix[int(weightIndices.x)] * weights.x;
//    matTransform += uBoneMatrix[int(weightIndices.y)] * weights.y;
//    matTransform += uBoneMatrix[int(weightIndices.z)] * weights.z;
//    matTransform += uBoneMatrix[int(weightIndices.w)] * finalWeight;
//
//    vec4 objPos = matTransform*vec4(aVertex, 1.0);
//
//    gl_Position    = uMVP*objPos;
//    vPosition      = objPos.xyz;
//    vNormal        = (matTransform*vec4(aNormal, 0.0)).xyz;
//    vTexCoord0     = aTexCoord;
//}

uniform mat2x4 uBoneDualQuat[MAX_BONES];

void main()
{
    float finalWeight = 1.0f - ( aBoneWeights.x + aBoneWeights.y + aBoneWeights.z );

	mat2x4 dq0 = uBoneDualQuat[aBoneIndices.x];
	mat2x4 dq1 = uBoneDualQuat[aBoneIndices.y];
	mat2x4 dq2 = uBoneDualQuat[aBoneIndices.z];
	mat2x4 dq3 = uBoneDualQuat[aBoneIndices.w];

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

    gl_Position    = uMVP * vec4(position, 1.0);
    vPosition      = position;
    vNormal        = normal;
    vTexCoord0     = aTexCoord;
}