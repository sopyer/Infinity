#version 330

#ifndef MAX_BONES
#define MAX_BONES 128
#endif 

layout(location=0) in vec3  aVertex;
layout(location=3) in ivec4 aBoneIndices;
layout(location=4) in vec4  aBoneWeights;

uniform mat4   uMVP;
uniform mat2x4 uBoneDualQuat[MAX_BONES];

out vec3 gaPosition;

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
	vec3 trans    = 2.0*(blendDQ[0].w*blendDQ[1].xyz - blendDQ[1].w*blendDQ[0].xyz + cross(blendDQ[0].xyz, blendDQ[1].xyz));
	position += trans;

    gl_Position = uMVP * vec4(position, 1.0);
    gaPosition  = position;
}