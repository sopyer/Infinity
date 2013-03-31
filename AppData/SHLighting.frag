#version 330

layout(binding=0) uniform sampler2D samDiffuse;
layout(binding=1) uniform sampler2D samNormal;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord0;

layout(location = 0, index = 0) out vec4 outColor;

mat3 cotangentFrame( vec3 N, vec3 p, vec2 uv )
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
    
    mat3 TBN = cotangentFrame(P, N.xyz, vTexCoord0);
    
    // Read normal map
    N.xyz = texture2D(samNormal, vTexCoord0).xyz;
    N.xyz = N.xyz * 255./127. - 128./127.;
    N.y   = -N.y;
    N.xyz = normalize(TBN * N.xyz);

    mat4 r_sh_matrix = mat4(
	     0.09010, -0.04719,  0.24026, -0.14838,
	    -0.04719, -0.09010, -0.11155,  0.19955,
	     0.24026, -0.11155, -0.11890, -0.17397,
	    -0.14838,  0.19955, -0.17397, -0.07239
	);
    mat4 g_sh_matrix = mat4(
	    -0.02145, -0.02145,  0.09010, -0.03070,
	    -0.02145,  0.02145, -0.09439,  0.17908,
	     0.09010, -0.09439, -0.06688, -0.09210,
	    -0.03070,  0.17908, -0.09210, -0.01280
	);
    mat4 b_sh_matrix = mat4(
	    -0.12871, -0.05149,  0.06007,  0.00512,
	    -0.05149,  0.12871, -0.20165,  0.30700,
	     0.06007, -0.20165, -0.11147, -0.13815,
	     0.00512,  0.30700, -0.13815, -0.03463
	);
	
	vec4 c;

    c.r = dot((r_sh_matrix * N), N);
    c.g = dot((g_sh_matrix * N), N);
    c.b = dot((b_sh_matrix * N), N);
    c.a = 1.0;
    
    c = 2.0*texture2D(samDiffuse, vTexCoord0);

	outColor = c;
}
