#include "Sphere.h"

Sphere::Sphere(const int iterations, GLuint& VBO_Sphere, const vec3 Position) :
	OpenGL_Object(Position, vec3(0.2f))
{
	VBO = &VBO_Sphere;
	vec3 Sphere_Core_Vertices[4] = {
		vec3(0.0, 0.0, 1.0),
		vec3(0.0, 0.942809, -0.333333),
		vec3(-0.816497, -0.471405, -0.333333),
		vec3(0.816497, -0.471405, -0.333333)
	};
	Sphere_Vertices.clear();
	DivideTriangle(Sphere_Core_Vertices[0], Sphere_Core_Vertices[1], Sphere_Core_Vertices[2], iterations);
	DivideTriangle(Sphere_Core_Vertices[0], Sphere_Core_Vertices[3], Sphere_Core_Vertices[1], iterations);
	DivideTriangle(Sphere_Core_Vertices[0], Sphere_Core_Vertices[2], Sphere_Core_Vertices[3], iterations);
	DivideTriangle(Sphere_Core_Vertices[3], Sphere_Core_Vertices[2], Sphere_Core_Vertices[1], iterations);

	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, Sphere_Vertices.size() * sizeof(VertexData), Sphere_Vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexData), (char*)(sizeof(vec3)));
	glEnableVertexAttribArray(1);
}
void Sphere::DivideTriangle(vec3 a, vec3 b, vec3 c, int itertions) {
	if (itertions > 0)
	{
		vec3 v1 = normalize(a + b);
		vec3 v2 = normalize(a + c);
		vec3 v3 = normalize(b + c);

		DivideTriangle(a, v1, v2, itertions - 1);
		DivideTriangle(v1, b, v3, itertions - 1);
		DivideTriangle(v1, v3, v2, itertions - 1);
		DivideTriangle(v2, v3, c, itertions - 1);
	}
	else
	{
		Sphere_Vertices.push_back(VertexData(a, a));
		Sphere_Vertices.push_back(VertexData(b, b));
		Sphere_Vertices.push_back(VertexData(c, c));
	}
}
void Sphere::BindSphere() {
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexData), (char*)(sizeof(vec3)));
	glEnableVertexAttribArray(1);
}
int Sphere::Size() {
	return Sphere_Vertices.size();
}