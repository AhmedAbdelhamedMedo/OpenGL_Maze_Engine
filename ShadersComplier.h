#pragma once
#include <gl/glew/glew.h>

static class ShadersComplier
{
	static char* readFile(const char* fileName);
	static bool compiledStatus(GLint shaderID);
	static GLuint makeShader(const char* ShaderSourceCode, GLenum S);
	static GLuint makeShaderProgram(GLuint vertShaderID, GLuint fragShaderID);
	static GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);
public:
	static void CompileShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name, GLuint& ProgramId);
};