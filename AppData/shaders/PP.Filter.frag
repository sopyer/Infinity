#version 430

#define MAX_SAMPLE_COUNT 64

layout(binding = 0) uniform sampler2D samSource;

layout(binding = 0) uniform uFilterDesc
{
    int   sampleCount;
    float weights[MAX_SAMPLE_COUNT];
    vec2  offsets[MAX_SAMPLE_COUNT];
};

in vec2 vUV;

layout(location = 0) out vec4 fragData;

void main(void)
{
    vec4 color = vec4(0, 0, 0, 0);

    for (int i=0; i<sampleCount; ++i)
    {
        color += weights[i]*texture2D(samSource, vUV+offsets[i]);
    }

    fragData = color;
}