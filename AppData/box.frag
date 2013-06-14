#version 330

#define UNI_LIGHTING  2

in vec3 vPosition;
in vec3 vABC;

layout(location = 0, index = 0) out vec4 outColor;

layout(binding = UNI_LIGHTING) uniform uniLighting
{
    mat4 uSHRed;
    mat4 uSHGreen;
    mat4 uSHBlue;
};

void main()
{
	vec3 P = vPosition;
	vec4 N = vec4(normalize(cross(dFdx(P), dFdy(P))), 1.0);
	vec4 c;
	
	N = vec4(normalize(vPosition), 1.0);
	
	N.xy = -N.xy;

	c.r = dot((uSHRed   * N), N);
	c.g = dot((uSHGreen * N), N);
	c.b = dot((uSHBlue  * N), N);
	c.a = 1.0;
    
	vec3  d  = fwidth(vABC);
	vec3  a3 = smoothstep(vec3(0.0), d*0.95, vABC);
	float t  = min(min(a3.x, a3.y), a3.z);
    
	//c.xyz = mix(vec3(0.0), c.xyz, t);

	outColor = c;
}
