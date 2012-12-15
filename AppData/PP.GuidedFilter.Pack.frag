varying vec2 vUV;

layout(binding = 0) uniform sampler2D samSource;
layout(binding = 1) uniform sampler2D samGuide;

void main()
{
    vec3  p = texture2D(samSource, vUV).xyz;
    float I = texture2D(samGuide,  vUV).x;
    
    vec4 pI    = vec4(p, I);
    
    gl_FragData[0] = pI;
    gl_FragData[1] = pI * I;
}