uniform sampler2D tex0;
uniform sampler2D tex1;

void main() {
	
	vec4 horizon = texture2D(tex0, gl_TexCoord[0].xy);
	vec4 clouds_0 = texture2D(tex1,gl_TexCoord[1].xy);
	vec4 clouds_1 = texture2D(tex1,gl_TexCoord[2].xy);
	
	vec4 clouds = (clouds_0 + clouds_1) * horizon.w;
	
	gl_FragColor = horizon * (1.0 - clouds.x) + clouds;
}