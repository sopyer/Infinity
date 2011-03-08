float perlin2D(vec2 P);
float fBm(vec2 p, int octaves, float amp, float ampScale, float freq, float freqScale);

uniform int   uOctaves;
uniform float uAmp;
uniform float uAmpScale;
uniform float uFreq;
uniform float uFreqScale;
uniform float uGamma;

void main(void)
{
	vec3 color = clamp(vec3(fBm(gl_TexCoord[0].st, uOctaves, uAmp, uAmpScale, uFreq, uFreqScale)*0.5+0.5), vec3(0.0), vec3(1.0));
	gl_FragColor = vec4(pow(color, vec3(uGamma)), 1);
}
