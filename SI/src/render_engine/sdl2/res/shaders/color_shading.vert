#version 130

in vec2 vertex_position;

void main()
{

    gl_Position.xy = vertex_position;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;
}