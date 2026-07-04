#include "OpenGL_Object.h"

OpenGL_Object::OpenGL_Object(const vec3 position, const vec3 rotation_axis, const float angle,
	const vec3 scale) {
	Position = position;
	Rotation_Axis = rotation_axis;
	Angle = angle;
	Scale = scale;
}
OpenGL_Object::OpenGL_Object(const vec3 position, const vec3 rotation_axis, const float angle) {
	Position = position;
	Rotation_Axis = rotation_axis;
	Angle = angle;
	Scale = vec3(1, 1, 1);
}
OpenGL_Object::OpenGL_Object(const vec3 position) {
	Position = position;
	Rotation_Axis = vec3(0, 1, 0);
	Angle = 0.0f;
	Scale = vec3(1, 1, 1);
}
OpenGL_Object::OpenGL_Object(const vec3 position, const vec3 scale) {
	Position = position;
	Rotation_Axis = vec3(0, 1, 0);
	Angle = 0.0f;
	Scale = scale;
}
OpenGL_Object::OpenGL_Object() {
	Position = vec3(0, 0, 0);
	Rotation_Axis = vec3(0, 1, 0);
	Angle = 0.0f;
	Scale = vec3(1, 1, 1);
}

OpenGL_Object::~OpenGL_Object() {}

vec3 OpenGL_Object::GetPosition() {
	return Position;
}
vec3 OpenGL_Object::GetRotation_Axis() {
	return Rotation_Axis;
}
float OpenGL_Object::GetAngle() {
	return Angle;
}
vec3 OpenGL_Object::GetScale() {
	return Scale;
}

void OpenGL_Object::SetPosition(const vec3 position) {
	Position = position;
}
void OpenGL_Object::SetRotation_Axis(const vec3 rotation_axis) {
	Rotation_Axis = rotation_axis;
}
void OpenGL_Object::SetAngle(const float angle) {
	Angle = angle;
}
void OpenGL_Object::SetScale(const vec3 scale) {
	Scale = scale;
}