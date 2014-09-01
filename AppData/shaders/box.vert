#version 430

layout(location=0) in vec3  aVertex;

out vec3 gaPosition;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * vec4(aVertex, 1.0);
    gaPosition  = aVertex;
}