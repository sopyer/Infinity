#version 330

uniform sampler1D samplerPerm;
uniform sampler1D samplerGrad;

float perm(float x)
{
	return texture1D(samplerPerm, x).r;
}

float grad(float x, vec2 p)
{
	return dot(texture1D(samplerGrad, x*8).rg, p);
}

vec2 fade(vec2 x)
{
	return x * x * x * (x * (x * 6 - 15) + 10); // new curve
//	return (3 - 2*x)*x*x;
}

const float texSize=256.0;
const float one = 1.0/texSize;

float perlin2D(vec2 P)
{
	vec2 Pi = floor(P)/texSize; // Integer part for permutation, divide by texture size for proper sampling
	vec2 Pf = fract(P); // Fractional part for interpolation
	
	float AA = perm(perm(Pi.x)+Pi.y);
	float AB = perm(perm(Pi.x)+Pi.y+one);
	float BA = perm(perm(Pi.x+one)+Pi.y);
	float BB = perm(perm(Pi.x+one)+Pi.y+one);
	
	vec2 f = fade(Pf);
	
	float noise = mix(
		mix(grad(AA, Pf),                   grad(BA, Pf+vec2(-1,  0)), f.x),
		mix(grad(AB, Pf+vec2(0, -1)),  grad(BB, Pf+vec2(-1, -1)), f.x),
		f.y
	);
	
	return noise;
}

// fractal sum
float fBm(vec2 p, int octaves, float lacunarity, float gain)
{
	float freq = 1.0, amp = 0.5;
	float sum = 0;	
	for(int i=0; i<octaves; i++)
	{
		sum += perlin2D(p*freq)*amp;
		freq *= lacunarity;
		amp *= gain;
	}
	return sum;
}
void main(void)
{
	gl_FragColor = vec4(vec3(fBm(gl_TexCoord[0].st*5, 8, 2.0, 0.5)*0.5+0.5), 1);
//	gl_FragColor = vec4(vec3(perlin2D(gl_TexCoord[0].st*5)*0.5+0.5), 1);
}
