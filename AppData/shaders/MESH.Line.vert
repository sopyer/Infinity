
struct line_t
{
    vec3 p0, p1;
};

layout(std140, column_major) uniform;

layout(binding = 0) uniform uniGlobal
{
    mat4  au_MVP;
    vec4  uColor;
    vec2  uPxScale;
    float uA;
};

layout(std140, binding = 1) buffer uniLines
{
    line_t uLines[];
};

out vec4 faColor;

const float THRESHOLD = 1e-3;

void main()
{
    uint lineIndex = gl_VertexID / 6;
    uint vertexIndex = abs(3 - gl_VertexID % 6);
    bool doSwap = bool(vertexIndex & 1);
    vec3 w0 = doSwap ? uLines[lineIndex].p1 : uLines[lineIndex].p0;
    vec3 w1 = doSwap ? uLines[lineIndex].p0 : uLines[lineIndex].p1;
    
    vec4 p0 = au_MVP * vec4(w0, 1.0);
    vec4 p1 = au_MVP * vec4(w1, 1.0);

    //Fix near plane intersection - place point on near plane if it is behind it;
    //Works only for non-reverse, non-infinite projection
    if (p0.z<0 && p1.z>0)
    {
        p0 = mix(p0, p1, abs(p0.z)/(abs(p0.z)+abs(p1.z)));
    }
    
    vec2 n = p1.xy/p1.w - p0.xy/p0.w;
    
    n = normalize(vec2(-n.y, n.x * uA));
    float s = bool(vertexIndex & 2) ? p0.w : -p0.w; // undo perspective and choose direction
    
    p0.xy += s * uPxScale * n;

    gl_Position = p0;
    faColor = uColor;
}