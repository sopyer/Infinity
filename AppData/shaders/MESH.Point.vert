
layout(std140, column_major) uniform;

layout(binding = 0) uniform uniGlobal
{
    mat4  au_MVP;
    vec4  uColor;
    vec2  uPxScale;
};

layout(std140, binding = 1) buffer uniLines
{
    vec3 uPoints[];
};

out vec4 faColor;

void main()
{
    vec4 p0 = au_MVP * vec4(uPoints[gl_VertexID / 6], 1.0);

    vec2 offset = uPxScale * -p0.w; //undo perspective

    uint vertexIndex = abs(3 - gl_VertexID % 6);
    p0.x += bool(vertexIndex & 1) ? -offset.x : offset.x;
    p0.y += bool(vertexIndex & 2) ? -offset.y : offset.y;

    gl_Position = p0;
    faColor = uColor;
}