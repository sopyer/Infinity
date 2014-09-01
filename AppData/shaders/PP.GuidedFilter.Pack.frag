#version 430

in vec2 vUV;

layout(binding = 0) uniform sampler2D samSource;
layout(binding = 1) uniform sampler2D samGuide;

layout(location = 0) out vec4 fragData0;
layout(location = 1) out vec4 fragData1;

void main()
{
    vec3  p = texture2D(samSource, vUV).xyz;
    float I = texture2D(samGuide,  vUV).x;

    vec4 pI    = vec4(p, I);

    fragData0 = pI;
    fragData1 = pI * I;
}