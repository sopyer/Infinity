#version 430

//Mapping distance between uStartPt and uEndPt in range [0..1]
//uDirection  = (uEndPt-uStartPt);
//uScale      = 1.0/dot(uDirection, uDirection);
//uDirection *= uScale;

uniform sampler1D   samColorRamp;

layout(std140, column_major, binding = 1) uniform PaintLinGradient
{
    uniform vec4   uStops [8];
    uniform vec4   uScales[8];
    uniform float  uInvStopCount;
    uniform vec2   uStartPoint;
    uniform vec2   uDirection;
};

in vec2 faPosition;

layout(location = 0, index = 0) out vec4 rt0;

vec4 evalGrad(float t)
{
    vec4 accum = vec4(0);

    for (int i=0; i<8; ++i)
    {
        vec4 deltas;

        // can not use multiply-add, because {0, 0.5, 0.5, 1} case would not work
        // also this case requires support of IEEE 754 floats as well
        // because uScales[1] is infinity in this case
        deltas = (vec4(t)-uStops[i])*uScales[i]; 
        deltas = clamp(deltas, vec4(0), vec4(1));
        accum += deltas;
    }

    float offset =(dot(accum, vec4(1))+0.5)*uInvStopCount;

    return texture(samColorRamp, offset);
}

void main()
{
    float t = dot(uDirection, faPosition-uStartPoint);
    rt0 = evalGrad(t);
}
