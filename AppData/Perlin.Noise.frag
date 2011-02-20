#version 330

uniform sampler1D samplerPerm;
uniform sampler1D samplerGrad;
uniform float     invTexDim;

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

float perlin2D(vec2 P)
{
	vec2 Pi = floor(P)*invTexDim; // Integer part for permutation, divide by texture size for proper sampling
	vec2 Pf = fract(P); // Fractional part for interpolation
	
	float AA = perm(perm(Pi.x)+Pi.y);
	float AB = perm(perm(Pi.x)+Pi.y+invTexDim);
	float BA = perm(perm(Pi.x+invTexDim)+Pi.y);
	float BB = perm(perm(Pi.x+invTexDim)+Pi.y+invTexDim);
	
	vec2 f = fade(Pf);
	
	float noise = mix(
		mix(grad(AA, Pf),                   grad(BA, Pf+vec2(-1,  0)), f.x),
		mix(grad(AB, Pf+vec2(0, -1)),  grad(BB, Pf+vec2(-1, -1)), f.x),
		f.y
	);
	
	return noise*0.4;
}

// fractal sum
float fBm(vec2 p, int octaves, float amp, float ampScale, float freq, float freqScale)
{
	float sum = 0;	
	for(int i=0; i<octaves; i++)
	{
		sum += perlin2D(p*freq)*amp;
		freq *= freqScale;
		amp *= ampScale;
	}
	return sum;
}
