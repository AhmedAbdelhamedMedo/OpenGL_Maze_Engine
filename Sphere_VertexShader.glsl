#version 330

layout (location = 0) in vec4 vertex_position;
layout (location = 1) in vec4 vertex_normal;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 proj_mat;
uniform vec3 camera_position;

vec3 light_direction = vec3(1, 1, 1);

out vec3 v; //View Direction
out vec3 n;	//Normal
out vec3 l;	//Light Direction
out vec3 h;	//Half Vector

void main()
{
	gl_Position = proj_mat * view_mat * model_mat * vertex_position;

	l = normalize(light_direction);
	mat4 normalMatrix = transpose(inverse(model_mat));
	n = normalize((normalMatrix * vertex_normal).xyz);
	v = normalize(camera_position - (model_mat * vertex_position).xyz);
	h = normalize(v+l);
}