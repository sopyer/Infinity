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
	vec3 uSHcoef[10];
};

vec4 evalSHPoly(vec3 N, vec3 shCoef[10])
{
    vec4 c;
    
    c.rgb  = shCoef[9];
    c.rgb += N.x * (shCoef[0] * N.x + (shCoef[3] * N.y + shCoef[6]));
    c.rgb += N.y * (shCoef[1] * N.y + (shCoef[4] * N.z + shCoef[7]));
    c.rgb += N.z * (shCoef[2] * N.z + (shCoef[5] * N.x + shCoef[8]));
    
    //c.rgb += N.x * shCoef[6];
    //c.rgb += N.y * shCoef[7];
    //c.rgb += N.z * shCoef[8];
    
    c.a = 1.0;
    
    return c;
}

void main()
{
    vec3 P = vPosition;
	vec4 N = vec4(normalize(cross(dFdx(P), dFdy(P))), 1.0);
	vec4 c;
	
    c.r = dot((uSHRed   * N), N);
    c.g = dot((uSHGreen * N), N);
    c.b = dot((uSHBlue  * N), N);
    c.a = 1.0;
    
    c = evalSHPoly(N.xyz, uSHcoef);

	vec3  d  = fwidth(vABC);
	vec3  a3 = smoothstep(vec3(0.0), d*0.95, vABC);
	float t  = min(min(a3.x, a3.y), a3.z);
    
	c.xyz = mix(vec3(0.0), c.xyz+0.3, t);

	outColor = c;
}
