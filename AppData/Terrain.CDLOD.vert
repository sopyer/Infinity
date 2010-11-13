uniform vec2	uOffset;
uniform vec2	uScale;
uniform vec3	uViewPos;
//Morph parameter are evaluated as follows:
//uMorphParam.x=1.0/(morphEnd-morphStart)
//uMorphParam.y=-morphStart/(morphEnd-morphStart)
uniform vec2	uMorphParams;

void main()
{
	vec2	gridPos = uOffset+uScale*gl_Vertex.xy;
	vec3	worldPos = vec3(gridPos.x, 0, gridPos.y);
	float	distance = length(worldPos-uViewPos);

	float	morphK = clamp(distance*uMorphParams.x+uMorphParams.y, 0.0, 1.0);
	
	//TODO: This is known during mesh creation, make it an attribute
	vec2	dir = fract(gl_Vertex.xy*vec2(0.5))*vec2(2.0);
	//TODO: Is there a way to move morphK*uScale out of shader?
	//TODO: make correct dir evaluation as 3d vector
	worldPos.xz += dir*vec2(morphK*uScale);

	gl_FrontColor = gl_BackColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix*vec4(worldPos, 1);
}

