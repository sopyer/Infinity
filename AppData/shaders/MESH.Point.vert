#version 430

layout(std140, column_major) uniform;

layout(binding = 0) uniform uniGlobal
{
    vec4  uColor;
    float uPixelScale;
    mat4  au_MV;
    vec4  au_Proj;
};

layout(std140, binding = 1) buffer uniLines
{
    vec3 uPoints[];
};

out vec4 faColor;

void main()
{
    vec3 viewPos = (au_MV * vec4(uPoints[gl_InstanceID], 1.0)).xyz;

    float scale = uPixelScale * -viewPos.z; //undo perspective

    viewPos.x += bool(gl_VertexID & 1) ? -scale : scale;
    viewPos.y += bool(gl_VertexID & 2) ? -scale : scale;

    gl_Position = vec4(viewPos.xy * au_Proj.xy, au_Proj.z*viewPos.z+au_Proj.w, -viewPos.z); //projection
    faColor = uColor;
}