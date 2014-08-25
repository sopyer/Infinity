#version 330

layout(binding=0) uniform sampler2D samDiffuse;

in vec3 vPosition;
in vec2 vTexCoord0;

layout(location = 0, index = 0) out vec4 fragColor;

void main()
{
    fragColor = texture2D(samDiffuse, vTexCoord0);
}
