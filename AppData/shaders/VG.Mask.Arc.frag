void main(void)
{
	vec2 uv = gl_TexCoord[0].st;

	if( (uv.s*uv.s + uv.t*uv.t)>1.0 )
		discard;

	gl_FragColor = vec4(1);
}
