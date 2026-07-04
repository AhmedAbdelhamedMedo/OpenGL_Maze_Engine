#include "FPCamera.h"

FPCamera::FPCamera(const vec3 StartPosition) {
	//cameraPos = vec3(0.0f, 0.0f, 3.0f);
	cameraPos = StartPosition;
	cameraFront = vec3(0.0f, 0.0f, -1.0f);
	cameraUp = vec3(0.0f, 1.0f, 0.0f);
	moveFront = cameraFront;
	yaw_Angle = -180.0f;
	pitch_Angle = 0.0f;
	sensitivity = 0.2f;
	cameraSpeed = 2.0f;
}

FPCamera::~FPCamera(){}

void FPCamera::Move(const char Direction, const float deltaTime, vector<OpenGL_Object>& Maze_Walls) {
	switch (Direction) {
		case ('w'): {
			//cameraPos += cameraSpeed * moveFront * deltaTime;
			vec3 velocity = moveFront * cameraSpeed;
			ResolveCollision(velocity, deltaTime, Maze_Walls);
			break;
		}
		case ('s'): {
			//cameraPos -= cameraSpeed * moveFront * deltaTime;
			vec3 velocity = -moveFront * cameraSpeed;
			ResolveCollision(velocity, deltaTime, Maze_Walls);
			break;
		}
		case ('a'): {
			//cameraPos -= normalize(cross(moveFront, cameraUp)) * cameraSpeed * deltaTime;
			vec3 right = -normalize(cross(moveFront, cameraUp));
			ResolveCollision(right * cameraSpeed, deltaTime, Maze_Walls);
			break;
		}
		case ('d'): {
			//cameraPos += normalize(cross(moveFront, cameraUp)) * cameraSpeed * deltaTime;
			vec3 right = normalize(cross(moveFront, cameraUp));
			ResolveCollision(right * cameraSpeed, deltaTime, Maze_Walls);
			break;
		}
	}
}

void FPCamera::ResolveCollision(const vec3 velocity, const float deltaTime, vector<OpenGL_Object>& Maze_Walls)
{
	vec3 NewCameraPosition = cameraPos;

	NewCameraPosition.x += velocity.x * deltaTime;
	if (GetCollision(NewCameraPosition, Maze_Walls))
		NewCameraPosition.x = cameraPos.x;

	NewCameraPosition.z += velocity.z * deltaTime;
	if (GetCollision(NewCameraPosition, Maze_Walls))
		NewCameraPosition.z = cameraPos.z;

	cameraPos = NewCameraPosition;
}

bool FPCamera::GetCollision(const vec3 NewCameraPosition, vector<OpenGL_Object>& Maze_Walls) {
	bool collisionZ = false;
	bool collisionX = false;
	const float Wall_HalfSize = 0.65f;
	for (int i = 0; i < Maze_Walls.size(); i++) {
		collisionX = NewCameraPosition.x + Wall_HalfSize >= Maze_Walls[i].GetPosition().x &&
			Maze_Walls[i].GetPosition().x + Wall_HalfSize >= NewCameraPosition.x;
		collisionZ = NewCameraPosition.z + Wall_HalfSize >= Maze_Walls[i].GetPosition().z &&
			Maze_Walls[i].GetPosition().z + Wall_HalfSize >= NewCameraPosition.z;
		if (collisionX && collisionZ) {
			return true;
		}
	}
	return false;
}

void FPCamera::Update(RenderWindow& window) {
	const Vector2i Mouse_Position = Mouse::getPosition(window);

	yaw_Angle += (Mouse_Position.x - (Utilities::Window_Width / 2.0f)) * sensitivity;
	pitch_Angle -= (Mouse_Position.y - (Utilities::Window_Height / 2.0f)) * sensitivity;

	if (pitch_Angle > 89.9f)
		pitch_Angle = 89.9f;
	if (pitch_Angle < -89.9f)
		pitch_Angle = -89.9f;

	direction.x = cos(radians(yaw_Angle)) * cos(radians(pitch_Angle));
	direction.y = sin(radians(pitch_Angle));
	direction.z = sin(radians(yaw_Angle)) * cos(radians(pitch_Angle));
	cameraFront = normalize(direction);
	moveFront = cameraFront;
	moveFront.y = 0.0f;
	if (length(moveFront) > 0.0001f)
		moveFront = normalize(moveFront);
}

void FPCamera::Render(GLuint& ShaderID, OpenGL_Object Obj) const {
	glUniformMatrix4fv(glGetUniformLocation(ShaderID, "view_mat"), 1, GL_FALSE, value_ptr(
		lookAt(cameraPos, cameraPos + cameraFront, cameraUp)));
	glUniformMatrix4fv(glGetUniformLocation(ShaderID, "proj_mat"), 1, GL_FALSE, value_ptr(
		perspectiveFov(FOV,
		(float)Utilities::Window_Width, (float)Utilities::Window_Height, 0.1f, 100.0f)));
	glUniformMatrix4fv(glGetUniformLocation(ShaderID, "model_mat"), 1, GL_FALSE, value_ptr(
		translate(Obj.GetPosition()) *
		rotate(Obj.GetAngle(), Obj.GetRotation_Axis()) *
		scale(Obj.GetScale())
	));
	glUniform3f(glGetUniformLocation(ShaderID, "camera_position"),
		cameraPos.x, cameraPos.y, cameraPos.z);
}

vec3 FPCamera::GetPosition() {
	return cameraPos;
}