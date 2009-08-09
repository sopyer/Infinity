void main(void)
{
	vec2 uv = gl_TexCoord[0].st;

	if( (pow(uv.s, 2.0)-uv.t)>0.0 )
		discard;

	gl_FragColor = vec4(1);
}
