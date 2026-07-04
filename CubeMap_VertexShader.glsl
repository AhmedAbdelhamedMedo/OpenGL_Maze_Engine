#version 330

layout (location = 0) in vec4 vertex_position;
layout (location = 1) in vec4 vertex_normal;
layout (location = 2) in vec2 vertex_UV;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 proj_mat;
uniform vec3 camera_position;

out vec3 UVW;

void main()
{
	gl_Position = proj_mat * view_mat * model_mat * vertex_position;

	UVW = vertex_position.xyz;
}