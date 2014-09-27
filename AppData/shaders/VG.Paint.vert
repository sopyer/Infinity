#version 430

layout(std140, column_major, binding = 0) uniform uniMVP
{
    mat4  uMVP;
};

layout(location=0) in vec2  vaPosition;

out vec2 faPosition;

void main()
{
   gl_Position = uMVP * vec4(vaPosition, 0.0, 1.0);
   faPosition  = vaPosition;
}
