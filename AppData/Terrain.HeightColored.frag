#version 330

in float vHeight;

void main()
{
	gl_FragColor = vec4(gl_Color.xyz*vec3(vHeight), 1);
}
