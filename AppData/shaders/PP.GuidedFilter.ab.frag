#version 430

in vec2 vUV;

layout(binding=0) uniform sampler2D samMeanPI;
layout(binding=1) uniform sampler2D samMeanPIII;

layout(location = 0) uniform float uEPS;

layout(location = 0) out vec4 fragData0;
layout(location = 1) out vec4 fragData1;
layout(location = 2) out vec4 fragData2;

void main(void)
{
    vec4  meanPI   = texture2D(samMeanPI,   vUV);
    vec4  meanPIII = texture2D(samMeanPIII, vUV);

    vec3  meanP   = meanPI.rgb;
    vec3  meanPxI = meanPIII.rgb;
    float meanI   = meanPI.a;
    float meanIxI = meanPIII.a;

    float varI   = meanIxI - meanI * meanI;
    vec3  covIxP = meanPxI - meanI * meanP;

    vec3  a = covIxP / (varI + uEPS);
    vec3  b = meanP - a * meanI;

    fragData0 = vec4(a, 1.0);
    fragData1 = vec4(b, 1.0);
}
