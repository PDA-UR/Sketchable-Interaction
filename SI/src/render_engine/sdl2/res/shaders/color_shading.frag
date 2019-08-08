#version 130

in vec2 fragment_position;
in vec4 fragment_color;
in vec2 fragment_uv;

out vec4 color;

uniform float time;
uniform sampler2D mysampler;

void main()
{
    vec4 texture_color = texture(mysampler, fragment_uv);

    color = texture_color;

    color *= vec4(fragment_color.r * (cos(fragment_position.x * 4 + time) + 1.0) * 0.5,
                 fragment_color.g * (cos(fragment_position.y + 8 + time) + 1.0) * 0.5,
                 fragment_color.b * (cos(fragment_position.x * 2 + time) + 1.0) * 0.5,
                 fragment_color.a);
}