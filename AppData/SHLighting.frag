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
    mat4 uSHRed;
    mat4 uSHGreen;
    mat4 uSHBlue;
};

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
    vec3 P = vPosition;
	vec4 N = vec4(normalize(cross(dFdx(P), dFdy(P))), 1.0);
    
    mat3 TBN = cotangentFrame(P, vNormal, vTexCoord0);
    
    // Read normal map
    N.xyz = texture2D(samNormal, vTexCoord0).xyz;
    N.xyz = N.xyz * 255./127. - 128./127.;
    N.y   = -N.y;
    N.xyz = normalize(TBN * N.xyz);

	vec4 c;

    c.r = dot((uSHRed   * N), N);
    c.g = dot((uSHGreen * N), N);
    c.b = dot((uSHBlue  * N), N);
    c.a = 1.0;
    
    c = 4.0*c*texture2D(samDiffuse, vTexCoord0)+0.1;
	
	outColor = c;
}
