float perlin2D(vec2 P);
float fBm(vec2 p, int octaves, float amp, float ampScale, float freq, float freqScale);

vec3 saturate(vec3 v)
{
	return clamp(v, vec3(0), vec3(1));
}

void main(void)
{
	vec3 color = saturate(vec3(fBm(gl_TexCoord[0].st, 6, 1.36, 0.75, 6.0, 2.0)*0.5+0.5));
	color = saturate(color-vec3(64/255.0));
	color = saturate(color*vec3(16*40.0/255.0));
	color = mix(vec3(255.0/255.0, 255.0/255.0, 255.0/255.0), vec3(208.0/255.0, 220.0/255.0, 224.0/255.0), color);
	gl_FragColor = vec4(color, 1);
//	gl_FragColor = vec4(vec3(perlin2D(gl_TexCoord[0].st*5)*0.5+0.5), 1);
}
