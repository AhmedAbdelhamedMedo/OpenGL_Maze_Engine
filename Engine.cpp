#include "Engine.h"

Engine::Engine() {
	Texture_Pathes[0] = "Textures/Wall.jpg";
	Texture_Pathes[1] = "Textures/Ground.jpg";

	Texture_Pathes[2] = "Textures/skybox/rt.png";
	Texture_Pathes[3] = "Textures/skybox/lf.png";
	Texture_Pathes[4] = "Textures/skybox/up.png";
	Texture_Pathes[5] = "Textures/skybox/dn.png";
	Texture_Pathes[6] = "Textures/skybox/bk.png";
	Texture_Pathes[7] = "Textures/skybox/ft.png";


	context.depthBits = 24;
	window = new RenderWindow(VideoMode(Utilities::Window_Width, Utilities::Window_Height),
		"OpenGL Maze", Style::Default, context);
	window->setPosition(Vector2i(100, 50));
	window->setFramerateLimit(60);
	window->setMouseCursorVisible(false);
	window->setVerticalSyncEnabled(true);

	glewInit();
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);

	LoadTextures();
	LoadShaders();

	CreateMaze();

	Camera = new FPCamera(Camera_Start_Position);
	deltaTime = 0.0f;
	Mouse::setPosition(Vector2i(window->getSize().x / 2, window->getSize().y / 2), *window);
	cout << "\n==== Press ESC to Quit ====\n";
}

void Engine::LoadTextures() {
	Image img;
	for (int i = 0; i < 2; i++) {
		if (!img.loadFromFile(Texture_Pathes[i])) {
			cout << "Error Loading Image";
			return;
		}
		else {
			glGenTextures(1, &Texture_ID[i]);
			glBindTexture(GL_TEXTURE_2D, Texture_ID[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
	}

	glGenTextures(1, &Texture_ID[2]);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Texture_ID[2]);

	for (int i = 0; i < 6; i++)
	{
		if (!img.loadFromFile(Texture_Pathes[i + 2]))
		{
			cout << "Error Loading Image";
			continue;
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
void Engine::LoadShaders() {
	ShadersComplier::CompileShader("Shaders/Maze_VertexShader.glsl", "Shaders/Maze_FragmentShader.glsl", Wall_ShaderID);
	ShadersComplier::CompileShader("Shaders/Maze_VertexShader.glsl", "Shaders/Maze_FragmentShader.glsl", Ground_ShaderID);
	ShadersComplier::CompileShader("Shaders/Sphere_VertexShader.glsl", "Shaders/Sphere_FragmentShader.glsl", Sphere_ShaderID);
	ShadersComplier::CompileShader("Shaders/CubeMap_VertexShader.glsl", "Shaders/CubeMap_FragmentShader.glsl", CubeMap_ShaderID);
}

void Engine::Input() {
	if (window->hasFocus()) {
		window->setMouseCursorVisible(false);
		if (Keyboard::isKeyPressed((Keyboard::W)) || Keyboard::isKeyPressed((Keyboard::Up))) {
			Camera->Move('w', deltaTime, Maze_Walls);
		}
		if (Keyboard::isKeyPressed((Keyboard::S)) || Keyboard::isKeyPressed((Keyboard::Down))) {
			Camera->Move('s', deltaTime, Maze_Walls);
		}
		if (Keyboard::isKeyPressed((Keyboard::A)) || Keyboard::isKeyPressed((Keyboard::Left))) {
			Camera->Move('a', deltaTime, Maze_Walls);
		}
		if (Keyboard::isKeyPressed((Keyboard::D)) || Keyboard::isKeyPressed((Keyboard::Right))) {
			Camera->Move('d', deltaTime, Maze_Walls);
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window->close();
		}
		if (Keyboard::isKeyPressed(Keyboard::Num1)) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		if (Keyboard::isKeyPressed(Keyboard::Num2)) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (Keyboard::isKeyPressed(Keyboard::Num3)) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			glPointSize(4);
		}
	}
	else {
		window->setMouseCursorVisible(true);
	}
}
void Engine::Run() {
	while (window->isOpen()) {
		deltaTime = clock.restart().asSeconds();
		Event event;
		while (window->pollEvent(event)) {
			switch (event.type) {
				case Event::Closed:
				{
					window->close();
					break;
				}
			}
		}
		Input();
		Update();
		Render();
		if (window->hasFocus())
			Mouse::setPosition(Vector2i(window->getSize().x / 2, window->getSize().y / 2), *window);
		window->display();
	}
}
void Engine::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	BindCubeMap();
	Camera->Render(CubeMap_ShaderID, OpenGL_Object(Camera_Start_Position, vec3(80)));
	glDrawElements(GL_TRIANGLES, 108, GL_UNSIGNED_INT, NULL);

	BindWall();
	for (int i = 0; i < Maze_Walls.size(); i++) {
		Camera->Render(Wall_ShaderID, Maze_Walls[i]);
		glDrawElements(GL_TRIANGLES, 108, GL_UNSIGNED_INT, NULL);
	}

	BindGround();
	for (int i = 0; i < Maze_Ground.size(); i++) {
		Camera->Render(Ground_ShaderID, Maze_Ground[i]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
	}

	glUseProgram(Sphere_ShaderID);
	Goal->BindSphere();
	Camera->Render(Sphere_ShaderID, OpenGL_Object(*Goal));
	glDrawArrays(GL_TRIANGLES, 0, Goal->Size());
}
void Engine::Update() {
	if (window->hasFocus())
		Camera->Update(*window);
	//float x = (Camera->GetPosition().x - Goal_Position.x) * (Camera->GetPosition().x - Goal_Position.x);
	//float y = (Camera->GetPosition().y - Goal_Position.y) * (Camera->GetPosition().y - Goal_Position.y);
	//float z = (Camera->GetPosition().z - Goal_Position.z) * (Camera->GetPosition().z - Goal_Position.z);
	//cout << sqrtf(x + y + z) << endl;
}

void Engine::CreateMaze() {
	Maze[1][9] = 2;
	Maze[9][1] = 3;

	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			switch (Maze[i][j])
			{
			case (0): {
				Maze_Ground.push_back(vec3(j, 0, i));
				break;
			}
			case (1): {
				Maze_Walls.push_back(vec3(j, 0, i));
				break;
			}
			case (2): {
				Camera_Start_Position = vec3(j, 0, i);
				Maze_Ground.push_back(vec3(j, 0, i));
				break;
			}
			case (3): {
				Goal_Position = vec3(j, -0.2f, i);
				Maze_Ground.push_back(vec3(j, 0, i));
				break;
			}
			default:
				break;
			}
		}
	}
	CreateWall();
	//BindWall();
	CreateGround();
	//BindGround();
	CreateCubeMap();
	Goal = new Sphere(5, VBO_Sphere, Goal_Position);
}
void Engine::CreateWall() {
	VertexData Wall_Vertices[] = {
		//Front
		{ vec3(-0.5f,  0.5f, 0.5f),		vec3(0, 0, 1),		vec2(0, 1) },
		{ vec3(-0.5f, -0.5f, 0.5f),		vec3(0, 0, 1),		vec2(0, 0) },
		{ vec3( 0.5f, -0.5f, 0.5f),		vec3(0, 0, 1),		vec2(1, 0) },
		{ vec3( 0.5f,  0.5f, 0.5f),		vec3(0, 0, 1),		vec2(1, 1) },

		//Right
		{ vec3(0.5f,  0.5f,  0.5f),		vec3(1, 0, 0),		vec2(0, 1) },
		{ vec3(0.5f, -0.5f,  0.5f),		vec3(1, 0, 0),		vec2(0, 0) },
		{ vec3(0.5f, -0.5f, -0.5f),		vec3(1, 0, 0),		vec2(1, 0) },
		{ vec3(0.5f,  0.5f, -0.5f),		vec3(1, 0, 0),		vec2(1, 1) },

		//Back
		{ vec3( 0.5f,  0.5f, -0.5f),	vec3(0, 0, -1),		vec2(0, 1) },
		{ vec3( 0.5f, -0.5f, -0.5f),	vec3(0, 0, -1),		vec2(0, 0) },
		{ vec3(-0.5f, -0.5f, -0.5f),	vec3(0, 0, -1),		vec2(1, 0) },
		{ vec3(-0.5f,  0.5f, -0.5f),	vec3(0, 0, -1),		vec2(1, 1) },

		//Left
		{ vec3(-0.5f,  0.5f, -0.5f),	vec3(-1, 0, 0),		vec2(0, 1) },
		{ vec3(-0.5f, -0.5f, -0.5f),	vec3(-1, 0, 0),		vec2(0, 0) },
		{ vec3(-0.5f, -0.5f,  0.5f),	vec3(-1, 0, 0),		vec2(1, 0) },
		{ vec3(-0.5f,  0.5f,  0.5f),	vec3(-1, 0, 0),		vec2(1, 1) },

		//Top
		{ vec3(-0.5f,  0.5f, -0.5f),	vec3(0, 1, 0),		vec2(0, 1) },
		{ vec3(-0.5f,  0.5f,  0.5f),	vec3(0, 1, 0),		vec2(0, 0) },
		{ vec3( 0.5f,  0.5f,  0.5f),	vec3(0, 1, 0),		vec2(1, 0) },
		{ vec3( 0.5f,  0.5f, -0.5f),	vec3(0, 1, 0),		vec2(1, 1) },

		//Bottom
		{ vec3(-0.5f, -0.5f,  0.5f),	vec3(0, -1, 0),		vec2(0, 1) },
		{ vec3(-0.5f, -0.5f, -0.5f),	vec3(0, -1, 0),		vec2(0, 0) },
		{ vec3( 0.5f, -0.5f, -0.5f),	vec3(0, -1, 0),		vec2(1, 0) },
		{ vec3( 0.5f, -0.5f,  0.5f),	vec3(0, -1, 0),		vec2(1, 1) },
	};

	GLuint Wall_Faces_Indices[] = {
	 0,  1,  2,   0,  2,  3,   // Front
	 4,  5,  6,   4,  6,  7,   // Right
	 8,  9, 10,   8, 10, 11,   // Back
	12, 13, 14,  12, 14, 15,   // Left
	16, 17, 18,  16, 18, 19,   // Top
	20, 21, 22,  20, 22, 23    // Bottom
	};

	glGenBuffers(1, &VBO_Wall);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Wall);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Wall_Vertices), Wall_Vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &IBO_Wall);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_Wall);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Wall_Faces_Indices), Wall_Faces_Indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexData), (char*)(sizeof(vec3)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(VertexData), (char*)(2 * sizeof(vec3)));
	glEnableVertexAttribArray(2);
}
void Engine::BindWall() {
	glUseProgram(Wall_ShaderID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture_ID[0]);
	glUniform1i(glGetUniformLocation(Wall_ShaderID, "Diffuse_texture"), 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Wall);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_Wall);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexData), (char*)(sizeof(vec3)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(VertexData), (char*)(2 * sizeof(vec3)));
	glEnableVertexAttribArray(2);
}
void Engine::CreateGround() {
	VertexData Ground_Vertices[] = {
		//Bottom
		{ vec3(-0.5f, -0.5f,  0.5f),	vec3(0, 1, 0),		vec2(0, 1) },
		{ vec3(-0.5f, -0.5f, -0.5f),	vec3(0, 1, 0),		vec2(0, 0) },
		{ vec3( 0.5f, -0.5f, -0.5f),	vec3(0, 1, 0),		vec2(1, 0) },
		{ vec3( 0.5f, -0.5f,  0.5f),	vec3(0, 1, 0),		vec2(1, 1) },
	};

	GLuint Ground_Faces_Indices[] = {
	 0,  1,  2,   0,  2,  3,   // Bottom
	};

	glGenBuffers(1, &VBO_Ground);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Ground);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Ground_Vertices), Ground_Vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &IBO_Ground);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_Ground);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Ground_Faces_Indices), Ground_Faces_Indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexData), (char*)(sizeof(vec3)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(VertexData), (char*)(2 * sizeof(vec3)));
	glEnableVertexAttribArray(2);
}
void Engine::BindGround() {
	glUseProgram(Ground_ShaderID);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Texture_ID[1]);
	glUniform1i(glGetUniformLocation(Ground_ShaderID, "Diffuse_texture"), 1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Ground);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_Ground);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexData), (char*)(sizeof(vec3)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(VertexData), (char*)(2 * sizeof(vec3)));
	glEnableVertexAttribArray(2);
}

void Engine::CreateCubeMap() {
	VertexData Cube_Vertices[] = {
		//Front
		{ vec3(-0.5f,  0.5f, 0.5f),		vec3(0, 0, 1),		vec2(0, 1) },
		{ vec3(-0.5f, -0.5f, 0.5f),		vec3(0, 0, 1),		vec2(0, 0) },
		{ vec3( 0.5f, -0.5f, 0.5f),		vec3(0, 0, 1),		vec2(1, 0) },
		{ vec3( 0.5f,  0.5f, 0.5f),		vec3(0, 0, 1),		vec2(1, 1) },

		//Right
		{ vec3(0.5f,  0.5f,  0.5f),		vec3(1, 0, 0),		vec2(0, 1) },
		{ vec3(0.5f, -0.5f,  0.5f),		vec3(1, 0, 0),		vec2(0, 0) },
		{ vec3(0.5f, -0.5f, -0.5f),		vec3(1, 0, 0),		vec2(1, 0) },
		{ vec3(0.5f,  0.5f, -0.5f),		vec3(1, 0, 0),		vec2(1, 1) },

		//Back
		{ vec3( 0.5f,  0.5f, -0.5f),	vec3(0, 0, -1),		vec2(0, 1) },
		{ vec3( 0.5f, -0.5f, -0.5f),	vec3(0, 0, -1),		vec2(0, 0) },
		{ vec3(-0.5f, -0.5f, -0.5f),	vec3(0, 0, -1),		vec2(1, 0) },
		{ vec3(-0.5f,  0.5f, -0.5f),	vec3(0, 0, -1),		vec2(1, 1) },

		//Left
		{ vec3(-0.5f,  0.5f, -0.5f),	vec3(-1, 0, 0),		vec2(0, 1) },
		{ vec3(-0.5f, -0.5f, -0.5f),	vec3(-1, 0, 0),		vec2(0, 0) },
		{ vec3(-0.5f, -0.5f,  0.5f),	vec3(-1, 0, 0),		vec2(1, 0) },
		{ vec3(-0.5f,  0.5f,  0.5f),	vec3(-1, 0, 0),		vec2(1, 1) },

		//Top
		{ vec3(-0.5f,  0.5f, -0.5f),	vec3(0, 1, 0),		vec2(0, 1) },
		{ vec3(-0.5f,  0.5f,  0.5f),	vec3(0, 1, 0),		vec2(0, 0) },
		{ vec3( 0.5f,  0.5f,  0.5f),	vec3(0, 1, 0),		vec2(1, 0) },
		{ vec3( 0.5f,  0.5f, -0.5f),	vec3(0, 1, 0),		vec2(1, 1) },

		//Bottom
		{ vec3(-0.5f, -0.5f,  0.5f),	vec3(0, -1, 0),		vec2(0, 1) },
		{ vec3(-0.5f, -0.5f, -0.5f),	vec3(0, -1, 0),		vec2(0, 0) },
		{ vec3( 0.5f, -0.5f, -0.5f),	vec3(0, -1, 0),		vec2(1, 0) },
		{ vec3( 0.5f, -0.5f,  0.5f),	vec3(0, -1, 0),		vec2(1, 1) },
	};

	GLuint Cube_Faces_Indices[] = {
	 0,  1,  2,   0,  2,  3,   // Front
	 4,  5,  6,   4,  6,  7,   // Right
	 8,  9, 10,   8, 10, 11,   // Back
	12, 13, 14,  12, 14, 15,   // Left
	16, 17, 18,  16, 18, 19,   // Top
	20, 21, 22,  20, 22, 23    // Bottom
	};

	glGenBuffers(1, &VBO_CubeMap);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_CubeMap);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Cube_Vertices), Cube_Vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &IBO_CubeMap);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_CubeMap);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Cube_Faces_Indices), Cube_Faces_Indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexData), (char*)(sizeof(vec3)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(VertexData), (char*)(2 * sizeof(vec3)));
	glEnableVertexAttribArray(2);
}
void Engine::BindCubeMap() {
	glUseProgram(CubeMap_ShaderID);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_CubeMap);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_CubeMap);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexData), (char*)(sizeof(vec3)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(VertexData), (char*)(2 * sizeof(vec3)));
	glEnableVertexAttribArray(2);
}