#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 gaPosition[3];

out vec3 vPosition;
out vec3 vABC;

void main()
{
    vPosition   = gaPosition[0];
    vABC        = vec3(1, 0, 0);
    gl_Position = gl_in[0].gl_Position; EmitVertex();

    vPosition   = gaPosition[1];
    vABC        = vec3(0, 1, 0);
    gl_Position = gl_in[1].gl_Position; EmitVertex();

    vPosition   = gaPosition[2];
    vABC        = vec3(0, 0, 1);
    gl_Position = gl_in[2].gl_Position; EmitVertex();

    EndPrimitive();
}
