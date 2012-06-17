layout(std140, column_major) uniform;

#define ATTR_POSITION	0
#define ATTR_PATCH_BASE	1
#define ATTR_LEVEL		2

layout(location = ATTR_POSITION)		in vec2		aVertex;

#ifdef ENABLE_INSTANCING
layout(location = ATTR_PATCH_BASE)		in vec2		aPatchBase;
layout(location = ATTR_LEVEL)			in float	aLevel;
#else
uniform uniPatch
{
	vec2  aPatchBase;
	float aLevel;
	float padding; //Just in case, AMD seems to use actual size and does not pad uniform blocks :(
};
#endif

#define MAX_LOD_COUNT 8

uniform uniTerrain
{
	vec4		uAABB;
	vec4		uUVXform;
	vec2		uHeightXform;

	float		uPatchScales[MAX_LOD_COUNT];
	// Morph parameter are evaluated as follows:
	// uMorphParam[...].x = 1.0/(morphEnd-morphStart)
	// uMorphParam[...].y = -morphStart/(morphEnd-morphStart)
	vec2		uMorphParams[MAX_LOD_COUNT];
	vec4		uColors[MAX_LOD_COUNT];
};

uniform uniView
{
	mat4		uMVP;
	vec4		uLODViewK;
};

uniform sampler2D	uHeightmap;

out float vHeight;
out vec4  vColor;
out vec3  vPos;
out vec2  vUV;

void main()
{
	float patchScale = uPatchScales[int(aLevel)];
	vec2  morphDir   = fract(aVertex*0.5)*2.0;

	vec3  vertexPos;

	vertexPos.xz = aPatchBase + patchScale*aVertex;
	vertexPos.xz = clamp(vertexPos.xz, uAABB.xy, uAABB.zw);
	vertexPos.y  = 0.0;

	float distance = length(vertexPos + uLODViewK.xyz);
	float morphK   = clamp(distance*uMorphParams[int(aLevel)].x + uMorphParams[int(aLevel)].y, 0.0, 1.0);

	vertexPos.xz += patchScale*morphDir*morphK; //Applying morphing for seamless connectivity

	vec2  uv = vertexPos.xz*uUVXform.xy+uUVXform.zw;
	float h  = textureLod(uHeightmap, uv, 0).r;

	vertexPos.y   = h*uHeightXform.x+uHeightXform.y;

	//Varyings

	gl_Position = uMVP*vec4(vertexPos, 1);
	vHeight     = h;
	vPos        = vertexPos;
	vColor      = uColors[int(aLevel.x)];
	vUV         = (aVertex + morphDir*morphK) / 8.0 * 64.0/8.0;
}

