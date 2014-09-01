#version 430

in vec4 faColor;

layout(location = 0, index = 0) out vec4 fragColor;

void main()
{
    fragColor = faColor;
}