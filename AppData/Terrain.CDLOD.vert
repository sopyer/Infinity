uniform vec3		uOffset;
uniform vec3		uScale;

uniform vec2		uPatchBase;
uniform vec2		uPatchDim;

uniform vec3		uViewPos;

uniform sampler2D	uHeightmap;
uniform vec2		uInvHMSize;

// Morph parameter are evaluated as follows:
// uMorphParam.x = 1.0/(morphEnd-morphStart)
// uMorphParam.y = -morphStart/(morphEnd-morphStart)
uniform vec2		uMorphParams;

float fetchHeight(vec2 gridPos)
{
//Add support for proper mipmapping
	vec2 tc = (gridPos+vec2(0.5))*uInvHMSize;
	return textureLod(uHeightmap, tc, 0).r;
}

vec3 getWorldPos(vec2 gridPos)
{
	return uOffset+uScale*vec3(gridPos.x, fetchHeight(gridPos), gridPos.y);
}

void main()
{
	vec2	patchPos, gridPos;
	vec3	worldPos;
	
	patchPos = gl_Vertex.xy;
	gridPos = uPatchBase+uPatchDim*patchPos;
	worldPos = getWorldPos(gridPos);
	
	//Applying morphing for seamless connectivity
	vec2 morphDir = fract(patchPos*vec2(0.5))*vec2(2.0);
	vec2 gridMorphDest = gridPos + uPatchDim*morphDir;
	vec3 worldMorphDest = getWorldPos(gridMorphDest);
	
	float	distance, morphK;

	distance = length(worldPos-uViewPos);
	morphK = clamp(distance*uMorphParams.x+uMorphParams.y, 0.0, 1.0);
	worldPos = mix(worldPos, worldMorphDest, morphK);
	
	gl_FrontColor = gl_BackColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix*vec4(worldPos, 1);
}

