varying float vZValue;

uniform sampler1D uVoxelLookupTexture;

void main()
{
	gl_FragColor = texture1D(uVoxelLookupTexture, vZValue);
}