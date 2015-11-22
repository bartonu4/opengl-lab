#version 330

in vec2 UV;

layout (location = 0) out vec3 color;
uniform sampler2D myTextureSampler;

void main(void)
{
    color = texture( myTextureSampler, UV ).rgb;
}
