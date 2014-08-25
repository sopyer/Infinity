#version 330

in vec3 vPosition;
in vec2 vTexCoord0;

layout(location = 0, index = 0) out vec4 fragColor;

void main()
{
    fragColor = vec4(1, 0, 0, 1);
}
