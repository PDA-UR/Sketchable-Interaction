#version 130

in vec2 vertex_position;
in vec4 vertex_color;

out vec4 fragment_color;

uniform mat4 P;

void main()
{
    gl_Position.xy = (P * vec4(vertex_position.x, vertex_position.y, 0.0, 1.0)).xy;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;

    fragment_color = vertex_color;
}