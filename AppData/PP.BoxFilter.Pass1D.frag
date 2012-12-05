uniform sampler2D samSource;

uniform int  kernelWidth;
uniform vec2 offsets;

in vec2 vUV;

void main(void)
{
	vec4 color = vec4(0, 0, 0, 0);

	for (int i=0; i<kernelWidth; ++i)
	{
		color += texture2D(samSource, vUV+offsets[i]);
	}
   
	gl_FragColor = color;
}