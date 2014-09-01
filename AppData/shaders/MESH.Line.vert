#version 430

struct line_t
{
    vec3 p0, p1;
};

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
    line_t uLines[];
};

out vec4 faColor;

void main()
{
    vec3 p0 = (au_MV * vec4(uLines[gl_InstanceID].p0, 1.0)).xyz;
    vec3 p1 = (au_MV * vec4(uLines[gl_InstanceID].p1, 1.0)).xyz;

    vec2 v = p1.xy - p0.xy;
    vec2 n = vec2(-v.y, v.x);

    vec3 viewPos;

    //normalize on square
    float s = max(abs(n.x), abs(n.y));
    n /= (s==0.0) ? 1.0 : s;

    viewPos     =  bool(gl_VertexID & 1) ? p0 : p1;
    viewPos.xy += (bool(gl_VertexID & 2) ? -n : n) * (uPixelScale * -viewPos.z); //undo perspective

    gl_Position = vec4(viewPos.xy * au_Proj.xy, au_Proj.z*viewPos.z+au_Proj.w, -viewPos.z); //projection
    faColor = uColor;
}