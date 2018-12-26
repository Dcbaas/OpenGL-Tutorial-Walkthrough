#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>

class Shader
{
public:
	Shader();

	void createFromString(const char* vertexCode, const char* fragmentCode);
	void create_from_file(const char* vert_filename, const char* frag_filename);
	std::string read_file(const char* filename);

	GLuint get_projection_location();
	GLuint get_model_location();

	void use_shader();
	void clear_shader();
	~Shader();

private:
	GLuint shaderID, uniformProjection, uniformModel;

	void compile_shader(const char* vertexCode, const char* fragmentCode);
	void add_shader(GLuint program, const char* shaderCode, GLenum shaderType);
};

