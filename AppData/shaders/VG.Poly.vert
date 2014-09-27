#version 430

layout(std140, column_major, binding = 0) uniform uniMVP
{
    mat4  uMVP;
};

layout(location=0) in vec3  aVertex;

out vec3  vPosition;

void main()
{
    gl_Position = uMVP * vec4(aVertex, 1.0);
}