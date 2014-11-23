
layout(std140, column_major, binding = 0) uniform uniMVP
{
    mat4  uMVP;
};

layout(location=0) in vec3  vaVertex;

#ifdef ENABLE_TEXTURE
layout(location=3) in vec2  vaUV0;
out vec2  faUV0;
#endif

#ifdef ENABLE_COLOR
layout(location=2) in vec4  vaColor;
out vec4  faColor;
#endif

void main()
{
    gl_Position = uMVP * vec4(vaVertex, 1.0);
#ifdef ENABLE_TEXTURE
    faUV0       = vaUV0;
#endif
#ifdef ENABLE_COLOR
    faColor     = vaColor;
#endif
}
