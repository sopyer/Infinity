#version 430

out vec2 vUV;

void main()
{
    vec2 pos;

    pos.x = bool(gl_VertexID & 1) ?  3.0 : -1.0;
    pos.y = bool(gl_VertexID & 2) ? -3.0 :  1.0;

    vUV         = pos * vec2(0.5, 0.5) + vec2(0.5, 0.5);
    gl_Position = vec4(pos, 0.0, 1.0);
}
