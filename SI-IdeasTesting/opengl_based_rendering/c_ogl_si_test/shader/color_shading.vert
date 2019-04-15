#version 130

in vec2 vertex_position;
in vec4 vertex_color;

out vec4 fragment_color;
out vec2 fragment_position;

void main()
{
    gl_Position.xy = vertex_position;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;

    fragment_position = vertex_position;

    fragment_color = vertex_color;
}
