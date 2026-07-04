#pragma once
#include "VertexData.h"
#include "ShadersComplier.h"
#include "OpenGL_Object.h"
#include "FPCamera.h"
#include "Sphere.h"
#include <iostream>

using namespace std;
using namespace sf;

class Engine {
	string Texture_Pathes[8];
	GLuint Texture_ID[3];

	GLuint Wall_ShaderID = 0;
	GLuint VBO_Wall = 0, IBO_Wall = 0;
	vector<OpenGL_Object> Maze_Walls;

	GLuint Ground_ShaderID = 0;
	GLuint VBO_Ground = 0, IBO_Ground = 0;
	vector<OpenGL_Object> Maze_Ground;

	GLuint Sphere_ShaderID = 0;
	GLuint VBO_Sphere = 0;
	Sphere* Goal;

	GLuint CubeMap_ShaderID = 0;
	GLuint VBO_CubeMap = 0, IBO_CubeMap = 0;

	FPCamera* Camera;
	vec3 Camera_Start_Position;
	vec3 Goal_Position;

	ContextSettings context;
	RenderWindow* window;
	Clock clock;
	float deltaTime;

	unsigned int Maze[11][11] = {
	{1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,1,0,0,0,0,0,1},
	{1,0,1,1,1,0,1,1,1,1,1},
	{1,0,1,0,0,0,1,0,0,0,1},
	{1,0,0,0,1,1,1,0,1,0,1},
	{1,0,1,1,1,0,0,0,1,0,1},
	{1,0,1,1,1,0,1,0,1,1,1},
	{1,0,1,1,0,0,1,0,0,0,1},
	{1,0,1,1,0,1,1,1,1,0,1},
	{1,0,0,0,0,1,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1}
	};

	void CreateMaze();
	void CreateWall();
	void BindWall();
	void CreateGround();
	void BindGround();
	void CreateCubeMap();
	void BindCubeMap();

	void LoadTextures();
	void LoadShaders();
	void Input();
	void Render();
	void Update();
public:
	Engine();
	void Run();
};