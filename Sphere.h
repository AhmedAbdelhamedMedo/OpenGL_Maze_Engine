#pragma once
#include "VertexData.h"
#include "OpenGL_Object.h"
#include <vector>

class Sphere : public OpenGL_Object {
	std::vector<VertexData> Sphere_Vertices;
	GLuint* VBO;
	void DivideTriangle(vec3 a, vec3 b, vec3 c, int itertions);
public:
	Sphere(const int iterations, GLuint& VBO_Sphere, const vec3 Position);
	void BindSphere();
	int Size();
};