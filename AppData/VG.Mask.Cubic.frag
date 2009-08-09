void main(void)
{
	vec3 uv = gl_TexCoord[0].stp;

	if( (pow(uv.s, 3.0)-uv.t*uv.p)>0.0 )
		discard;

	gl_FragColor = vec4(1);
}
