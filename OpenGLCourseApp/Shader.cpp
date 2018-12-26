#include "Shader.h"



Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::createFromString(const char* vertexCode, const char* fragmentCode)
{
	compile_shader(vertexCode, fragmentCode);
}

void Shader::create_from_file(const char* vert_filename, const char* frag_filename)
{
	std::string vertexString = read_file(vert_filename);
	std::string fragment_string = read_file(frag_filename);
	const char* vertex_code = vertexString.c_str();
	const char* fragment_code = fragment_string.c_str();

	compile_shader(vertex_code, fragment_code);
}

std::string Shader::read_file(const char* filename)
{
	std::string content;
	std::ifstream file_stream(filename, std::ios::in);

	if(!file_stream.is_open())
	{
		printf("Failed to read %s! File does not exist.", filename);
	}

	std::string line = "";
	while(!file_stream.eof())
	{
		std::getline(file_stream, line);
		content.append(line + "\n");
	}
	file_stream.close();
	return content;
}

GLuint Shader::get_projection_location()
{
	return uniformProjection;
}

GLuint Shader::get_model_location()
{
	return uniformModel;
}

void Shader::compile_shader(const char* vertexCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID) {
		std::cout << "ERROR creating Shader Program" << std::endl;
		//This isn't error safe. make this safer later.
		return;
	}

	add_shader(shaderID, vertexCode, GL_VERTEX_SHADER);
	std::cout << "Vertex Shader done" << std::endl;
	add_shader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);
	std::cout << "Fragment Shader" << std::endl;

	//Verify the Shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		std::cout << "Error linking program" << std::endl << eLog << std::endl;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		std::cout << "Error validating program" << std::endl << eLog << std::endl;
	}

	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
}

void Shader::add_shader(GLuint program, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	//Verify the Shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(theShader, sizeof(eLog), NULL, eLog);
		std::cout << "Error compiling the shader " << shaderType << eLog << std::endl;
	}

	glAttachShader(program, theShader);
}

void Shader::use_shader()
{
	glUseProgram(shaderID);
}

void Shader::clear_shader()
{
	if(shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
}

Shader::~Shader()
{
	clear_shader();
}
