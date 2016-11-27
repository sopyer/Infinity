#version 430

layout(binding=0) uniform sampler1D samPerm;
layout(binding=1) uniform sampler1D samGrad;

uniform float invTexDim;

float perm(float x)
{
    return texture(samPerm, x).r;
}

float grad(float x, vec2 p)
{
    return dot(texture(samGrad, x*8).rg, p);
}

vec2 fade(vec2 x)
{
    return x * x * x * (x * (x * 6 - 15) + 10); // new curve
//  return (3 - 2*x)*x*x;
}

float perlin2D(vec2 P)
{
    vec2 Pi = floor(P)*invTexDim; // Integer part for permutation, divide by texture size for proper sampling
    vec2 Pf = fract(P);               // Fractional part for interpolation

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

    return noise*0.27;
}

// fractal sum
float fBm(vec2 p, int octaves, float amp, float ampScale, float freq, float freqScale)
{
    float sum = 0;
    for(int i=0; i<octaves; i++)
    {
        sum  += amp * perlin2D(p*freq);
        freq *= freqScale;
        amp  *= ampScale;
    }
    return sum;
}
