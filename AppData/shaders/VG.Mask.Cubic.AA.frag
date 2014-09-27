#version 430

in vec3  vTexCoord0;

layout(location = 0, index = 0) out vec4 rt0;

void main(void)
{
    vec3   uv = vTexCoord0.stp;
    vec3   dF = vec3(3*uv.x*uv.x, -uv.z, -uv.y);
    vec2   grad = vec2(dot(dFdx(uv), dF), dot(dFdy(uv), dF));
    float  F = uv.x*uv.x*uv.x - uv.y*uv.z;
    float  sdist = 0.5-F/length(grad);

    if (sdist < 0.0000152587890625)
        discard;

    float  a = clamp(sdist, 0.0, 1.0);

    rt0 = vec4(1.0, 1.0, 1.0, a);
}
