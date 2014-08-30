#version 440

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
    vec4 viewP = uMV * vec4(aVertex, 1.0);
    vec4 viewN = uMV * vec4(aNormal, 0.0);

    gl_Position = vec4(viewP.xy * uProj.xy, uProj.z*viewP.z+uProj.w, -viewP.z); //projection
    vPosition   = viewP.xyz;
    vNormal     = viewN.xyz;
    vTexCoord0  = aUV0;
}