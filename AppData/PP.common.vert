#version 330

layout(location = 0) in vec2 aVertex;

out vec2 vUV;

void main()
{
    vUV         = aVertex * vec2(0.5, 0.5) + vec2(0.5, 0.5);
    gl_Position = vec4(aVertex, 0.0, 1.0);
}
