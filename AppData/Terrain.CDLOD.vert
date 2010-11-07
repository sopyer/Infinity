uniform vec2	uOffset;
uniform vec2	uScale;
//TODO: uViewPos should be 3d
uniform vec2	uViewPos;
//Morph parameter are evaluated as follows:
//uMorphParam.x=1.0/(morphEnd-morphStart)
//uMorphParam.y=-morphStart/(morphEnd-morphStart)
uniform vec2	uMorphParams;

void main()
{
	//TODO: use 3d vectors for positions
	vec2	worldPos = uOffset+uScale*gl_Vertex.xy;
	float	distance = length(worldPos-uViewPos);

	float	morphK = clamp(distance*uMorphParams.x+uMorphParams.y, 0.0, 1.0);
	
	//TODO: This is known during mesh creation, make it an attribute
	vec2	dir = fract(gl_Vertex.xy*vec2(0.5))*vec2(2.0);
	//TODO: Is there a way to move morphK*uScale out of shader?
	worldPos += dir*vec2(morphK*uScale);

	gl_FrontColor = gl_BackColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix*vec4(worldPos, 0, 1);
}

