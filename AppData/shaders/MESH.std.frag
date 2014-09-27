
#ifdef ENABLE_TEXTURE
layout(binding=0) uniform sampler2D samDiffuse;
in vec2  faUV0;
#endif

#ifdef ENABLE_COLOR
in vec4  faColor;
#endif

layout(location = 0, index = 0) out vec4 rt0;

void main()
{
    vec4 color = vec4(1.0);

#ifdef ENABLE_COLOR
    color  = faColor;
#endif
#ifdef ENABLE_TEXTURE
    color *= texture(samDiffuse, faUV0);
#endif

    rt0 = color;
}