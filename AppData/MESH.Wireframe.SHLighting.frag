#version 330

#define UNI_LIGHTING  2

in vec3 vPosition;
in vec3 vABC;

layout(location = 0, index = 0) out vec4 outColor;

layout(binding = UNI_LIGHTING) uniform uniLighting
{
    vec3 au_SHcoef[10];
};

vec4 evalSH2(vec3 N, vec3 shCoef[10])
{
    vec4 c;
    
    c.rgb  = shCoef[9];
    c.rgb += N.x * (shCoef[0] * N.x + (shCoef[3] * N.y + shCoef[6]));
    c.rgb += N.y * (shCoef[1] * N.y + (shCoef[4] * N.z + shCoef[7]));
    c.rgb += N.z * (shCoef[2] * N.z + (shCoef[5] * N.x + shCoef[8]));

    c.a = 1.0;

    return c;
}

void main()
{
    vec3 P = vPosition;
    vec3 N = normalize(cross(dFdx(P), dFdy(P)));
    vec4 c = evalSH2(N, au_SHcoef);

    vec3  d  = fwidth(vABC);
    vec3  a3 = smoothstep(vec3(0.0), d*0.95, vABC);
    float t  = min(min(a3.x, a3.y), a3.z);

    c.xyz = mix(vec3(0.0), c.xyz, t);

    outColor = c;
}
