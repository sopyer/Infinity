void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = vec4(gl_Vertex.xy, 0, 0);
}
