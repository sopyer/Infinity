#version 430

layout(std140, column_major, binding = 1) uniform PaintSolid
{
    uniform vec4 uPaintColor;
};

layout(location = 0, index = 0) out vec4 rt0;

void main()
{
    rt0 = uPaintColor;
}
