#version 430

in vec2 vUV;

layout(binding = 0) uniform sampler2D samSource;
layout(binding = 1) uniform sampler2D samGuide;

layout(location = 0) out vec4 fragData0;
layout(location = 1) out vec4 fragData1;
layout(location = 2) out vec4 fragData2;

void main()
{
    vec3 p = texture2D(samSource, vUV).xyz;
    vec3 I = texture2D(samGuide,  vUV).xyz;

    fragData0 = vec4(I * p.r, p.r);
    fragData1 = vec4(I * p.g, p.g);
    fragData2 = vec4(I * p.b, p.b);
}