#version 330

#define UNI_STD_TRANSFORMS  0

layout(std140, column_major) uniform;

layout(location=0) in vec3  aVertex;
layout(location=1) in vec3  aNormal;
layout(location=2) in vec2  aUV0;

layout(binding = UNI_STD_TRANSFORMS) uniform uniStdTransforms
{
    mat4 uMV;
    vec4 uProj;
};

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoord0;

void main()
{
    vec4 viewPos = uMV * vec4(aVertex, 1.0);

    gl_Position = vec4(viewPos.xy * uProj.xy, uProj.z*viewPos.z+uProj.w, -viewPos.z); //projection
    vPosition   = aVertex;
    vNormal     = aNormal;
    vTexCoord0  = aUV0;
}