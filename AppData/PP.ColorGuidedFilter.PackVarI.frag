#version 330

in vec2 vUV;

layout(binding = 0) uniform sampler2D samGuide;

void main()
{
    vec3  I = texture2D(samGuide, vUV).xyz;
    
    gl_FragData[0] = vec4(I.xyz * I.xyz, 1.0);
    gl_FragData[1] = vec4(I.xxy * I.yzz, 1.0);
}