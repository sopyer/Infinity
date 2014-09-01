#version 430

layout(location=0) in vec3 aVertex;

out vec3 vDir;

void main()
{
	gl_Position   = gl_ModelViewProjectionMatrix * vec4(aVertex, 0.0);
	gl_Position.w = gl_Position.z;
	vDir =  vec3(aVertex.xy, -aVertex.z);
}