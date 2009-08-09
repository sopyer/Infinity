sampler2D	uImage;

void main()
{
	gl_FragColor = tex2D(uImage, gl_TexCoord[0].xy);
}
