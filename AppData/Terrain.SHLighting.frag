#version 330

in float vHeight;
in vec3  vPos;
in vec2  vUV;

layout(binding = 1) uniform sampler2D uMipTexture;

layout(location = 0, index = 0) out vec4 outColor;

void main()
{
    //TODO: implement correct gamma aware rendering
    //imitate gamma
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

    vec4 normal = vec4(normalize(cross(dFdx(vPos), dFdy(vPos))), 1.0);
    vec4 c;

    c.r = dot((r_sh_matrix * normal), normal);
    c.g = dot((g_sh_matrix * normal), normal);
    c.b = dot((b_sh_matrix * normal), normal);
    c.a = 1.0;

    vec4 mip = texture(uMipTexture, vUV);
    c = mix(sqrt(c+0.2), mip, mip.a);

    outColor = c;
}
