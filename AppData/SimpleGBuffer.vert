varying vec3 vPos;
varying vec3 vNormal;

void main()
{
	vPos = (gl_ModelViewMatrix*gl_Vertex).xyz;
	vNormal = normalize(gl_NormalMatrix*gl_Normal);
	
	gl_Position = ftransform();
}