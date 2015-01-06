layout(std140, column_major, binding = 0) uniform uniMVP
{
    mat4  uMVP;
};

layout(location=0) in vec4  vaRect;
layout(location=1) in vec4  vaColor;

out vec4 faColor;

void main()
{
    float x = ((gl_VertexID & 1) != 0) ? vaRect.x : vaRect.z;
    float y = ((gl_VertexID & 2) != 0) ? vaRect.y : vaRect.w;

    gl_Position = uMVP * vec4(x, y, 0, 1);
    faColor     = vaColor;
}
