varying vec3 vPos;
varying vec3 vNormal;

void main()
{
	gl_FragData[0] = vec4(vPos, 1);
	gl_FragData[1] = vec4(vNormal, 1);
}