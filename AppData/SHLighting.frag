#version 330

#define UNI_LIGHTING  2

layout(binding=0) uniform sampler2D samDiffuse;
layout(binding=1) uniform sampler2D samNormal;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord0;

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

mat3 cotangentFrame( vec3 p, vec3 N, vec2 uv )
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );
 
    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
 
    // construct a scale-invariant frame 
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}

void main()
{
    mat3 TBN = cotangentFrame(vPosition, vNormal, vTexCoord0);
	vec3 N;
    
    // Read normal map
    N   = texture2D(samNormal, vTexCoord0).xyz;
    N   = N * 255./127. - 128./127.;
    N.y = -N.y;
    N   = normalize(TBN * N);

	vec4 c = evalSHPoly(N, uSHcoef);

    c = 4.0*c*texture2D(samDiffuse, vTexCoord0)+0.1;
	
	outColor = c;
}
