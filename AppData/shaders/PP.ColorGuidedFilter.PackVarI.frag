#version 440

in vec2 vUV;

layout(binding = 0) uniform sampler2D samGuide;

layout(location = 0) out vec4 fragData0;
layout(location = 1) out vec4 fragData1;

void main()
{
    vec3  I = texture2D(samGuide, vUV).xyz;

    fragData0 = vec4(I.xyz * I.xyz, 1.0);
    fragData1 = vec4(I.xxy * I.yzz, 1.0);
}