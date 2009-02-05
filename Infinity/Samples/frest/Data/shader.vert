void main(void)
{
	vec4	t;
	vec3	p, n, r;
	
	t = gl_ModelViewMatrix * gl_Vertex;	p = t.xyz;			//transform position
	t = gl_ModelViewMatrix * vec4(gl_Normal, 0); n = t.xyz;	//rotate normal
	r = p - 2.0 * dot(n, p) * n;	//reflected vector(pointing in the same direction as normal);
	r.z += 1.0;
	r = normalize(r);
	
	gl_Position    = ftransform();
    gl_FrontColor  =  gl_BackColor = gl_Color;
    gl_TexCoord[0] = vec4(r.xy * 0.5 + 0.5, 0, 0);
}