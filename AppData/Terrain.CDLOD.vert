//#version 150

uniform vec2		uOffset;
uniform vec2		uPatchBase;
uniform vec2		uPatchDim;
uniform vec2		uScale;
uniform vec3		uViewPos;
uniform sampler2D	uHeightmap;

//Morph parameter are evaluated as follows:
//uMorphParam.x=1.0/(morphEnd-morphStart)
//uMorphParam.y=-morphStart/(morphEnd-morphStart)
uniform vec2		uMorphParams;

float fetchHeight(vec2 gridPos)
{
	return textureLod(uHeightmap, (gridPos+vec2(0.5))/vec2(2305, 2945)*16, 0).x;
}

vec3 getWorldPos(vec2 gridPos)
{
	vec3 worldPos;
	
	worldPos.xz = uOffset+uScale*gridPos;
	worldPos.y = fetchHeight(gridPos)*63;
	
	return worldPos;
}

void main()
{
	vec2	patchPos, gridPos;
	vec3	worldPos;
	
	patchPos = gl_Vertex.xy;
	gridPos = uPatchBase+uPatchDim*patchPos;
	worldPos = getWorldPos(gridPos);
	
	//Applying morphing for seamless connectivity
	float	distance, morphK;
	vec2	morphDir;
	
	distance = length(worldPos-uViewPos);
	morphK = clamp(distance*uMorphParams.x+uMorphParams.y, 0.0, 1.0);
	morphDir = fract(patchPos*vec2(0.5))*vec2(2.0);

	vec2 gridMorphDest = gridPos + uPatchDim*morphDir;
	vec3 worldMorphDest = getWorldPos(gridMorphDest);
	
	worldPos = mix(worldPos, worldMorphDest, morphK);
	
	gl_FrontColor = gl_BackColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix*vec4(worldPos, 1);
}

