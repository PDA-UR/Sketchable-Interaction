#version 130

// input from vbo. each vertex is 2 floats
in vec2 vertex_position;
in vec4 vertex_color;
in vec2 vertex_uv;

out vec4 fragment_color;
out vec2 fragment_position;
out vec2 fragment_uv;

uniform mat4 P;

void main()
{

    gl_Position.xy = (P * vec4(vertex_position.x, vertex_position.y, 0.0, 1.0)).xy;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;

    fragment_color = vertex_color;
    fragment_position = vertex_position;
    fragment_uv = vec2(vertex_uv.x, 1.0 - vertex_uv.y);
}