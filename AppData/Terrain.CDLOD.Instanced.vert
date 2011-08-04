#version 330

#define ATTR_POSITION	0
#define ATTR_PATCH_BASE	1
#define ATTR_LEVEL		2

layout(location = ATTR_POSITION)		in vec2		aVertex;
layout(location = ATTR_PATCH_BASE)		in vec2		aPatchBase;
layout(location = ATTR_LEVEL)			in float		aLevel;

#define MAX_LOD_COUNT 8

uniform vec3		uOffset;
uniform vec3		uScale;

uniform vec3		uViewPos;
uniform mat4		uMVP;

uniform sampler2D	uHeightmap;
uniform vec4		uHMDim;

// Morph parameter are evaluated as follows:
// uMorphParam[...].x = 1.0/(morphEnd-morphStart)
// uMorphParam[...].y = -morphStart/(morphEnd-morphStart)
uniform vec2		uMorphParams[MAX_LOD_COUNT];
uniform vec3		uColors[MAX_LOD_COUNT];

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
	vec2	patchScale = vec2(1<<int(aLevel));
	
	patchPos = aVertex;
	gridPos = aPatchBase+patchScale*patchPos;
	worldPos = getWorldPos(gridPos);
	
	//Applying morphing for seamless connectivity
	vec2 morphDir = fract(patchPos*vec2(0.5))*vec2(2.0);
	vec2 gridMorphDest = gridPos + patchScale*morphDir;
	vec3 worldMorphDest = getWorldPos(gridMorphDest);
	
	float	distance, morphK;

	distance = length(worldPos-uViewPos);
	morphK = clamp(distance*uMorphParams[int(aLevel)].x+uMorphParams[int(aLevel)].y, 0.0, 1.0);
	worldPos = mix(worldPos, worldMorphDest, morphK);
	
	vHeight = worldPos.y/uScale.y-uOffset.y;

	vColor = vec4(uColors[int(aLevel)], 1);
	gl_Position = uMVP*vec4(worldPos, 1);
}

