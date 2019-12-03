#version 130

in vec4 fragment_color;
in vec2 fragment_uv;

out vec4 color;

uniform sampler2D mysampler;

void main()
{
    color = texture(mysampler, fragment_uv);
}