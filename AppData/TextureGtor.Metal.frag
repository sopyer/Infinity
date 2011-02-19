sampler1D samplerPerm;
sampler1D samplerGrad;

float perlin(vec2 uv)
{
	//~ // Compute what gradients to use
	//~ int qx0 = (int)floorf(x);
	//~ int qx1 = qx0 + 1;
	//~ float tx0 = x - (float)qx0;
	//~ float tx1 = tx0 - 1;

	//~ int qy0 = (int)floorf(y);
	//~ int qy1 = qy0 + 1;
	//~ float ty0 = y - (float)qy0;
	//~ float ty1 = ty0 - 1;

	//~ // Make sure we don't come outside the lookup table
	//~ qx0 = qx0 % size;
	//~ qx1 = qx1 % size;

	//~ qy0 = qy0 % size;
	//~ qy1 = qy1 % size;

	vec2 q0 = floor(x);
	vec2 q1 = q0+vec2(1, 1);
	
	vec2 t0 = uv-q0;
	vec2 t1 = t0-vec2(1, 1);

	//~ // Permutate values to get pseudo randomly chosen gradients
	//~ int q00 = permutation[(qy0 + permutation[qx0]) % size];
	//~ int q01 = permutation[(qy0 + permutation[qx1]) % size];

	//~ int q10 = permutation[(qy1 + permutation[qx0]) % size];
	//~ int q11 = permutation[(qy1 + permutation[qx1]) % size];

	float q00 = texture1D(samplerPerm, texture1D(samplerPerm, q0.x).r+q0.y).r;
	float q01 = texture1D(samplerPerm, texture1D(samplerPerm, q1.x).r+q0.y).r;
	float q10 = texture1D(samplerPerm, texture1D(samplerPerm, q0.x).r+q1.y).r;
	float q11 = texture1D(samplerPerm, texture1D(samplerPerm, q1.x).r+q1.y).r;

	//~ // Compute the dotproduct between the vectors and the gradients
	//~ float v00 = gradient[q00].x*tx0 + gradient[q00].y*ty0;
	//~ float v01 = gradient[q01].x*tx1 + gradient[q01].y*ty0;

	//~ float v10 = gradient[q10].x*tx0 + gradient[q10].y*ty1;
	//~ float v11 = gradient[q11].x*tx1 + gradient[q11].y*ty1;

	float v00 = texture1D(samplerGrad, q00).r*t0.x+texture1D(samplerGrad, q00).r*t0.y;
	float v01 = texture1D(samplerGrad, q01).r*t1.x+texture1D(samplerGrad, q01).r*t0.y;
	float v10 = texture1D(samplerGrad, q10).r*t0.x+texture1D(samplerGrad, q10).r*t1.y;
	float v11 = texture1D(samplerGrad, q11).r*t1.x+texture1D(samplerGrad, q11).r*t1.y;

	//~ // Modulate with the weight function
	//~ float wx = (3 - 2*tx0)*tx0*tx0;
	//~ float v0 = v00 - wx*(v00 - v01);
	//~ float v1 = v10 - wx*(v10 - v11);

	//~ float wy = (3 - 2*ty0)*ty0*ty0;
	//~ float v = v0 - wy*(v0 - v1);

	// Modulate with the weight function
	float wx = (3 - 2*t0.x)*t0.x*t0.x;
	float v0 = v00 - wx*(v00 - v01);
	float v1 = v10 - wx*(v10 - v11);

	float wy = (3 - 2*t0.y)*t0.y*t0.y0;
	float v = v0 - wy*(v0 - v1);
	
	return v;
}

void main(void)
{
//	vec2 uv = gl_TexCoord[0].st;

//	gl_FragColor = vec4(uv.x*uv.y);
	gl_FragColor = vec4(perlin(gl_TexCoord[0].st));
}
