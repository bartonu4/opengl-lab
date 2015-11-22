#version 330

uniform mat4 model_matrix;
uniform mat4 projection_matrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv_coor;

out vec2  UV;

void main(void)
{
    
    gl_Position = projection_matrix * (model_matrix * vec4(position,1));
	UV = uv_coor;
}
