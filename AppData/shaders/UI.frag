uniform vec4 fillColor /*= vec4( 1.0, 0.0,0.0,1.0)*/;
uniform vec4 borderColor /*= vec4( 1.0, 1.0,1.0,1.0)*/;
uniform vec2 zones;

void main()
{
	float doTurn = float(gl_TexCoord[0].y > 0);
	float radiusOffset = doTurn * abs( gl_TexCoord[0].z );
	float turnDir = sign( gl_TexCoord[0].z );
	vec2 uv = vec2(gl_TexCoord[0].x + turnDir*radiusOffset, gl_TexCoord[0].y);
	float l = abs( length(uv) - radiusOffset );
	float a = clamp( l - zones.x, 0.0, 2.0);
	float b = clamp( l - zones.y, 0.0, 2.0);
	b = exp2(-2.0*b*b);
	gl_FragColor = ( fillColor * b + (1.0-b)*borderColor );
	gl_FragColor.a *= exp2(-2.0*a*a);
}
