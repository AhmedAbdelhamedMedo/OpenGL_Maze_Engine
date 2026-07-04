#version 330

in vec3 UVW;

uniform samplerCube cube_map;

void main()
{
	gl_FragColor = texture(cube_map, UVW);
}