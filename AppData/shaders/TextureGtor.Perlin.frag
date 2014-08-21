#version 440

in vec2 vUV;

float perlin2D(vec2 P);
float fBm(vec2 p, int octaves, float amp, float ampScale, float freq, float freqScale);

uniform int   uOctaves;
uniform float uAmp;
uniform float uAmpScale;
uniform float uFreq;
uniform float uFreqScale;

layout(location = 0) out vec4 fragData;

void main(void)
{
    vec3 color = clamp(vec3(fBm(vUV, uOctaves, uAmp, uAmpScale, uFreq, uFreqScale)*0.5+0.5), vec3(0.0), vec3(1.0));
    fragData = vec4(color, 1.0);
}
