#version 430

in vec3  vTexCoord0;

layout(location = 0, index = 0) out vec4 rt0;

void main(void)
{
    vec3 uv = vTexCoord0.stp;

    if( (pow(uv.s, 3.0)-uv.t*uv.p)>0.0 )
        discard;

    rt0 = vec4(1.0);
}
