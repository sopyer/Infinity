varying float vZValue;

uniform sampler1D uVoxelLookupTexture;

void main()
{
	gl_FragColor = tex1D(uVoxelLookupTexture, vZValue);
}