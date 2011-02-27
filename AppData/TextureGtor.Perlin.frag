float perlin2D(vec2 P);
float fBm(vec2 p, int octaves, float amp, float ampScale, float freq, float freqScale);

uniform int   uOctaves;
uniform float uAmp;
uniform float uAmpScale;
uniform float uFreq;
uniform float uFreqScale;


void main(void)
{
//	gl_FragColor = vec4(vec3(fBm(gl_TexCoord[0].st, 6, 1.36, 0.75, 6.0, 2.0)*0.5+0.5), 1);
	gl_FragColor = vec4(vec3(fBm(gl_TexCoord[0].st, uOctaves, uAmp, uAmpScale, uFreq, uFreqScale)*0.5+0.5), 1);
}
