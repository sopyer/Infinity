#version 440

in vec2 vUV;

layout(binding=0) uniform sampler2D samMeanI;
layout(binding=1) uniform sampler2D samMeanI_xx_yy_zz;
layout(binding=2) uniform sampler2D samMeanI_xy_xz_yz;
layout(binding=3) uniform sampler2D samMeanIr_r;
layout(binding=4) uniform sampler2D samMeanIg_g;
layout(binding=5) uniform sampler2D samMeanIb_b;

layout (location = 0) uniform float uEPS;

layout(location = 0) out vec4 fragData0;
layout(location = 1) out vec4 fragData1;
layout(location = 2) out vec4 fragData2;

void main(void)
{
    vec3  meanI          = texture2D(samMeanI,          vUV).xyz;
    vec3  meanI_xx_yy_zz = texture2D(samMeanI_xx_yy_zz, vUV).xyz;
    vec3  meanI_xy_xz_yz = texture2D(samMeanI_xy_xz_yz, vUV).xyz;
    vec4  meanIr_r       = texture2D(samMeanIr_r,       vUV);
    vec4  meanIg_g       = texture2D(samMeanIg_g,       vUV);
    vec4  meanIb_b       = texture2D(samMeanIb_b,       vUV);
    
    float cov_xx = meanI_xx_yy_zz.x - meanI.x * meanI.x;
    float cov_yy = meanI_xx_yy_zz.y - meanI.y * meanI.y;
    float cov_zz = meanI_xx_yy_zz.z - meanI.z * meanI.z;

    float cov_xy = meanI_xy_xz_yz.x - meanI.x * meanI.y;
    float cov_xz = meanI_xy_xz_yz.y - meanI.x * meanI.z;
    float cov_yz = meanI_xy_xz_yz.z - meanI.y * meanI.z;

    mat3x3 sigma = mat3x3(
        cov_xx+uEPS, cov_xy,      cov_xz,
        cov_xy,      cov_yy+uEPS, cov_yz,
        cov_xz,      cov_yz,      cov_zz+uEPS
    );

    sigma = inverse(sigma);
    
    vec3  covIr = meanIr_r.xyz - meanIr_r.w * meanI;
    vec3  Ar    = covIr * sigma;
    float Br    = meanIr_r.w - dot(Ar, meanI);

    vec3  covIg = meanIg_g.xyz - meanIg_g.w * meanI;
    vec3  Ag    = covIg * sigma;
    float Bg    = meanIg_g.w - dot(Ag, meanI);

    vec3  covIb = meanIb_b.xyz - meanIb_b.w * meanI;
    vec3  Ab    = covIb * sigma;
    float Bb    = meanIb_b.w - dot(Ab, meanI);

    fragData0 = vec4(Ar, Br);
    fragData1 = vec4(Ag, Bg);
    fragData2 = vec4(Ab, Bb);
}
