#pragma once
#include "Utilities.h"
#include "OpenGL_Object.h"
#include <vector>
using namespace std;
class FPCamera {
	const float FOV = 60.0f;

	vec3 cameraPos;
	vec3 cameraFront;
	vec3 cameraUp;
	vec3 direction;
	vec3 moveFront;

	float yaw_Angle;
	float pitch_Angle;
	float cameraSpeed;
	float sensitivity;

	void ResolveCollision(const vec3 velocity, const float deltaTime, vector<OpenGL_Object>& Maze_Walls);
	bool GetCollision(const vec3 NewCameraPosition, vector<OpenGL_Object>& Maze_Walls);
public:
	FPCamera(const vec3 StartPosition);
	~FPCamera();
	void Update(RenderWindow& window);
	void Move(const char Direction, const float deltaTime, vector<OpenGL_Object>& Maze_Walls);
	void Render(GLuint& ShaderID, OpenGL_Object Obj) const;
	vec3 GetPosition();
};