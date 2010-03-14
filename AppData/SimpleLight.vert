varying vec4 vColor;

void main()
{
	gl_Position = ftransform();
	vec3 normal = normalize(gl_NormalMatrix*gl_Normal);
	float intesity = max(dot(normal, vec3(1, 1, 0)), 0.14);
	vColor = vec4(intesity, intesity, intesity, 1.0);
}