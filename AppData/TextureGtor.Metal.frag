uniform sampler2D samInput1;
uniform sampler2D samInput2;

uniform mat4 uHSCB;

vec3 saturate(vec3 v)
{
	return clamp(v, vec3(0), vec3(1));
}

vec3 AdjustColor(vec3 color)
{
	color = saturate(color-vec3(64/255.0));
	color = saturate(color*vec3(16*40.0/255.0));
	color = mix(vec3(255.0/255.0, 255.0/255.0, 255.0/255.0), vec3(208.0/255.0, 220.0/255.0, 224.0/255.0), color);
	
	return color;
}
void main(void)
{
	vec3 color1 = AdjustColor(texture2D(samInput1, gl_TexCoord[0].st).xyz);
	vec3 color2 = AdjustColor(texture2D(samInput2, gl_TexCoord[0].st).xyz);

	gl_FragColor = uHSCB*vec4((color1-color2*0.75)*0.25+0.2, 1);
}
