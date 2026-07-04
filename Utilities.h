#pragma once
#include <gl/glew/glew.h>
#include "gl/glm/glm.hpp"
#include <gl\glm\gtc\type_ptr.hpp>
#include <gl\glm\gtx\transform.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

using namespace glm;
using namespace sf;

static class Utilities {
public:
	static const unsigned int Window_Width = 1700;
	static const unsigned int Window_Height = 850;
};