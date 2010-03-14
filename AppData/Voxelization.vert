varying float vZValue;

void main()
{
	vec4 p = gl_ModelViewProjectionMatrix*gl_Vertex;//ftransform();
	vZValue = (p.z/p.w)*0.5+0.5;
	gl_Position = p;
}