#pragma once
#include "Utilities.h"

struct VertexData {
	vec3 Position;
	vec3 Normal;
	vec2 UV;

	VertexData(const vec3& Position, const vec3& Normal, const vec2& UV)
		: Position(Position), Normal(Normal), UV(UV)
	{ }

	VertexData(const vec3& Position, const vec3& Normal)
		: Position(Position), Normal(Normal)
	{
		UV = vec2(0, 0);
	}
};