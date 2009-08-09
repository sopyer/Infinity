uniform float time;

void main() {
	
	gl_Position = ftransform();
	
	gl_TexCoord[0] = vec4(0.0,1.0 - gl_Vertex.z / 600.0 + 0.3,0.0,1.0);
	
	gl_TexCoord[1] = vec4(gl_Vertex.x / 2000.0 + time / 80.0,gl_Vertex.y / 2000.0,0.0,1.0);
	gl_TexCoord[2] = vec4(gl_Vertex.x / 1000.0 + time / 90.0,gl_Vertex.y / 1000.0,0.0,1.0);
}