
struct line_t
{
    vec3 p0, p1;
};

layout(std140, column_major) uniform;

layout(binding = 0) uniform uniGlobal
{
    vec4  uColor;
    mat4  au_MV;
    vec4  au_Proj;
    float uPixelScale;
    float uZn;
};

layout(std140, binding = 1) buffer uniLines
{
    line_t uLines[];
};

out vec4 faColor;

const float THRESHOLD = 1e-3;

void main()
{
    vec3 p0 = (au_MV * vec4(uLines[gl_InstanceID].p0, 1.0)).xyz;
    vec3 p1 = (au_MV * vec4(uLines[gl_InstanceID].p1, 1.0)).xyz;

    vec3 v = p1 - p0;

    vec3 viewPos =  bool(gl_VertexID & 1) ? p0 : p1;

    //Fix near plane intersection - place point on near plane if it is behind it;
    if (viewPos.z>0 && p0.z*p1.z<0)
    {
        v /= v.z;
        viewPos -= (viewPos.z - uZn) * v;
    }

    //normalize on square
    float s = max(abs(v.x), abs(v.y));
    s = (s>0.0) ? 1.0/s : 1.0;

    vec2 n = s * vec2(-v.y, v.x);
    viewPos.xy += (bool(gl_VertexID & 2) ? -n : n) * (uPixelScale * -viewPos.z); //undo perspective

    gl_Position = vec4(viewPos.xy * au_Proj.xy, au_Proj.z*viewPos.z+au_Proj.w, -viewPos.z); //projection
    faColor = uColor;
}