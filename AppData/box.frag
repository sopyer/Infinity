#version 330

layout(std140, column_major) uniform;

#define UNI_LIGHTING  2

in vec3 vPosition;
in vec3 vABC;

layout(location = 0, index = 0) out vec4 outColor;

layout(binding = UNI_LIGHTING) uniform uniLighting
{
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
	
	N = vec4(normalize(vPosition), 1.0);
	
	vec3  d  = fwidth(vABC);
	vec3  a3 = smoothstep(vec3(0.0), d*0.95, vABC);
	float t  = min(min(a3.x, a3.y), a3.z);
    
	//c.xyz = mix(vec3(0.0), c.xyz, t);
    
    c = evalSHPoly(N.xyz, uSHcoef);
    
	outColor = c;
}
