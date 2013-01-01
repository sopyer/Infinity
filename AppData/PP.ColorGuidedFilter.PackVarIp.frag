varying vec2 vUV;

layout(binding = 0) uniform sampler2D samSource;
layout(binding = 1) uniform sampler2D samGuide;

void main()
{
    vec3 p = texture2D(samSource, vUV).xyz;
    vec3 I = texture2D(samGuide,  vUV).xyz;
    
    gl_FragData[0] = vec4(I * p.r, p.r);
    gl_FragData[1] = vec4(I * p.g, p.g);
    gl_FragData[2] = vec4(I * p.b, p.b);
}