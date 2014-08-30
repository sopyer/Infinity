#version 440

#define UNI_LIGHTING  1

layout(binding=0) uniform sampler2D samDiffuse;

in vec3 vPosition;
in vec2 vTexCoord0;

layout(location = 0, index = 0) out vec4 fragColor;

void main()
{
    vec4 diffuse = texture2D(samDiffuse, vTexCoord0);
    
    if (diffuse.a < 0.5f) discard;

    fragColor = diffuse;
}
