#version 330

in vec2 vUV;

layout(binding=0) uniform sampler2D samGuide;
layout(binding=1) uniform sampler2D samMeanABx;
layout(binding=2) uniform sampler2D samMeanABy;
layout(binding=3) uniform sampler2D samMeanABz;

void main(void)
{
    vec3  I       = texture2D(samGuide,   vUV).rgb;
    vec4  meanABx = texture2D(samMeanABx, vUV);
    vec4  meanABy = texture2D(samMeanABy, vUV);
    vec4  meanABz = texture2D(samMeanABz, vUV);
    
    gl_FragColor = vec4(
        dot(meanABx.xyz, I) + meanABx.w,
        dot(meanABy.xyz, I) + meanABy.w,
        dot(meanABz.xyz, I) + meanABz.w,
        1.0
    );
}