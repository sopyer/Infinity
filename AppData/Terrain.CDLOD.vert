uniform vec2	uOffset;
uniform vec2	uScale;
uniform vec3	uViewPos;
//Morph parameter are evaluated as follows:
//uMorphParam.x=1.0/(morphEnd-morphStart)
//uMorphParam.y=-morphStart/(morphEnd-morphStart)
uniform vec2	uMorphParams;

void main()
{
	vec2	patchPos, gridPos;
	vec3	worldPos;
	
	patchPos = gl_Vertex.xy;
	gridPos = uOffset+uScale*gl_Vertex.xy;
	worldPos = vec3(gridPos.x, 0, gridPos.y);
	
	float	distance, morphK;
	vec2	morphDir;
	
	distance = length(worldPos-uViewPos);
	morphK = clamp(distance*uMorphParams.x+uMorphParams.y, 0.0, 1.0);
	morphDir = fract(patchPos*vec2(0.5))*vec2(2.0);

	//Applying morphing for seamless connectivity
	//TODO: make correct dir evaluation as 3d vector
	worldPos.xz += morphDir*vec2(morphK*uScale);

	gl_FrontColor = gl_BackColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix*vec4(worldPos, 1);
}

