layout(std140, column_major) uniform;

#define ATTR_POSITION	0
#define ATTR_PATCH_BASE	1
#define ATTR_LEVEL		2

layout(location = ATTR_POSITION)		in vec2		aVertex;

#ifdef ENABLE_INSTANCING
layout(location = ATTR_PATCH_BASE)		in vec2		aPatchBase;
layout(location = ATTR_LEVEL)			in vec2		aLevel;
#else
uniform uniPatch
{
	vec2 aPatchBase;
	vec2 aLevel;  //Just in case, AMD seems to use actual size and does not pad uniform blocks :(
};
#endif

#define MAX_LOD_COUNT 8

uniform uniTerrain
{
	vec4		uHMDim;
	vec3		uOffset;
	vec3		uScale;

	// Morph parameter are evaluated as follows:
	// uMorphParam[...].x = 1.0/(morphEnd-morphStart)
	// uMorphParam[...].y = -morphStart/(morphEnd-morphStart)
	vec2		uMorphParams[MAX_LOD_COUNT];

	vec3		uColors[MAX_LOD_COUNT];
};

uniform uniView
{
	mat4		uMVP;
	vec4		uViewPos;
};

uniform sampler2D	uHeightmap;

float fetchHeight(vec2 gridPos)
{
//Add support for proper mipmapping
	vec2 tc = (gridPos+vec2(0.5))*uHMDim.zw;
	return textureLod(uHeightmap, tc, 0).r;
}

vec3 getWorldPos(vec2 gridPos)
{
	//clamping to match terrain dimentions
	gridPos = clamp(gridPos, vec2(0), uHMDim.xy-vec2(1.0));
	return uOffset+uScale*vec3(gridPos.x, fetchHeight(gridPos), gridPos.y);
}

out float vHeight;
out vec4  vColor;

void main()
{
	vec2	patchPos, gridPos;
	vec3	worldPos;
	vec2	patchScale = vec2(1<<int(aLevel.x));
	
	patchPos = aVertex;
	gridPos = aPatchBase+patchScale*patchPos;
	worldPos = getWorldPos(gridPos);
	
	//Applying morphing for seamless connectivity
	vec2 morphDir = fract(patchPos*vec2(0.5))*vec2(2.0);
	vec2 gridMorphDest = gridPos + patchScale*morphDir;
	vec3 worldMorphDest = getWorldPos(gridMorphDest);
	
	float	distance, morphK;

	distance = length(worldPos-uViewPos.xyz);
	morphK = clamp(distance*uMorphParams[int(aLevel.x)].x+uMorphParams[int(aLevel.x)].y, 0.0, 1.0);
	worldPos = mix(worldPos, worldMorphDest, morphK);
	
	vHeight = worldPos.y/uScale.y-uOffset.y;

	vColor = vec4(uColors[int(aLevel.x)], 1);
	gl_Position = uMVP*vec4(worldPos, 1);
}

