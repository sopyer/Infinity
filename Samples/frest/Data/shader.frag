uniform sampler2D reflection;
uniform sampler2D highlight;

void main()
{
	vec4	color1 = texture2D(reflection, gl_TexCoord[0].xy);
	vec4	color2 = texture2D(highlight, gl_TexCoord[0].xy);
	
    gl_FragColor = color1 * gl_Color + color2;
}