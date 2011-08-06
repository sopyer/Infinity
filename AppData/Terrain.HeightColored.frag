#version 330

in float vHeight;

uniform uniGradient
{
	uniform vec4		uStops[8];
	uniform vec4		uScales[8];
	uniform float		uInvStopCount;
};

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
		deltas = (vec4(t)-uStops[i])*uScales[i];
		deltas = clamp(deltas, vec4(0), vec4(1));
		accum += deltas;
	}

	float offset =(dot(accum, vec4(1))+0.5)*uInvStopCount;
	
	return texture(samColorRamp, offset);
}

layout(location = 0, index = 0) out vec4 outColor;

void main()
{
	outColor = vec4(evalGrad(vHeight).xyz, 1);
}
