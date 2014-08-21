#version 120

uniform vec4		uStops[8];
uniform vec4		uScales[8];
uniform float		uInvStopCount;
uniform sampler1D	samColorRamp;

vec4 evalGrad(float t)
{
	vec4 accum = vec4(0);
	vec4 deltas;

	for (int i=0; i<8; ++i)
	{
		// can not use multiply-add, because {0, 0.5, 0.5, 1} case would not work 
		// also this case requires support of IEEE 754 floats support for this as well
		// because uScales[1] is infinity in this case
		deltas = (t.xxxx-uStops[i])*uScales[i];
		deltas = clamp(deltas, vec4(0), vec4(1));
		accum += deltas;
	}

	float offset =(dot(accum, vec4(1))+0.5)*uInvStopCount;
	
	return texture(samColorRamp, offset);
}

void main()
{
	const vec2 p1 = vec2(440, 0);
	const vec2 p2 = vec2(840, 0);

	vec2 d = p2-p1;
	float t = dot(d, gl_FragCoord.xy-vec2(440, 0))/dot(d, d);
	gl_FragColor = evalGrad(t);
}
