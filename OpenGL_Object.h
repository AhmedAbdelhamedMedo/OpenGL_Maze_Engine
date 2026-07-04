#pragma once
#include "Utilities.h"

class OpenGL_Object
{
	vec3 Position;
	vec3 Rotation_Axis;
	vec3 Scale;
	float Angle;
public:
	OpenGL_Object(const vec3 position, const vec3 rotation_axis, const float angle, const vec3 scale);
	OpenGL_Object(const vec3 position, const vec3 rotation_axis, const float angle);
	OpenGL_Object(const vec3 position);
	OpenGL_Object(const vec3 position, const vec3 scale);
	OpenGL_Object();
	~OpenGL_Object();

	vec3 GetPosition();
	vec3 GetRotation_Axis();
	float GetAngle();
	vec3 GetScale();

	void SetPosition(const vec3 position);
	void SetRotation_Axis(const vec3 rotation_axis);
	void SetAngle(const float angle);
	void SetScale(const vec3 scale);
};