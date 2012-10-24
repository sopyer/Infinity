#version 330

uniform sampler2D uTexture;

in vec3 vPosition;
in vec3 vABC;

layout(location = 0, index = 0) out vec4 outColor;

void main()
{
    vec3 P = vPosition;
	vec4 N = vec4(normalize(cross(dFdx(P), dFdy(P))), 1.0);

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

	//vec3  d = fwidth(vABC);
	//vec3  a = smoothstep(vec3(0.0), d*0.95, vBC);
	//float t = min(min(a3.x, a3.y), a3.z);

	//c = mix(vec3(0.0), c+0.3, edgeFactor());
	//gl_FragColor.a = 1.0;

	outColor = c + 0.3;
}
