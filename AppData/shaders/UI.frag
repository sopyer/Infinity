#version 430

layout (location =  0) uniform vec4 uFillColor;
layout (location =  1) uniform vec4 uBorderColor;
layout (location =  2) uniform vec2 uZones;

in vec3  vTexCoord0;

layout(location = 0, index = 0) out vec4 rt0;

void main()
{
    float doTurn       = float(vTexCoord0.y>0);
    float radiusOffset = doTurn*abs(vTexCoord0.z);
    float turnDir      = sign(vTexCoord0.z);
    vec2  uv = vec2(vTexCoord0.x+turnDir*radiusOffset, vTexCoord0.y);
    float l  = abs(length(uv) - radiusOffset);
    float a  = clamp(l - uZones.x, 0.0, 2.0);
    float b  = clamp(l - uZones.y, 0.0, 2.0);

    b = exp2(-2.0*b*b);

    rt0    = (uFillColor*b + (1.0-b)*uBorderColor);
    rt0.a *= exp2(-2.0*a*a);
}
