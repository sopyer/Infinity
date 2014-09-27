#version 430

layout(std140, column_major, binding = 0) uniform uniMVP
{
    mat4  uMVP;
};

layout(location=0) in vec3  aVertex;
layout(location=1) in vec3  aUV0;

out vec3  vTexCoord0;

void main()
{
    gl_Position = uMVP * vec4(aVertex, 1.0);
    vTexCoord0  = aUV0;
}