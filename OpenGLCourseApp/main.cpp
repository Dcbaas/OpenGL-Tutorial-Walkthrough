#include <iostream>
#include <string.h>
#include <stdio.h>
#include <cmath>

//include the normal string lib for if it can be used instead
#include <string>
#include <GL/glew.h>
#include <GlfW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.1159265f / 180.0f;

float currAngle = 0.0f;

bool sizeDir = true;
float currSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

//For the first triangle we are drawing these are global variable
//In future find out how not to use global variables
GLuint VAO, VBO, IBO, shader , uniformModel, uniformProjecion; 

bool direction = true; 
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

//Create a vertex shader. Nomally this would be done with a file but not for this case. 
static const char* vShader = "                                                \n\
#version 330                                                                  \n\
                                                                              \n\
layout (location = 0) in vec3 pos;											  \n\
out vec4 vCol;																	\n\
                                                                              \n\
uniform mat4 model;                                                          \n\
                                                                              \n\
uniform mat4 projection;														\n\
																			\n\
void main()                                                                   \n\
{                                                                             \n\
    gl_Position = projection * model * vec4(pos, 1.0);										\n\
	vCol = vec4(clamp(pos,0.0f, 1.0f), 1.0f);											\n\
}";

//Fragment Shader
static const char* fShader = "						\n\
#version 330										\n\
													\n\
													\n\
													\n\
in vec4 vCol;										\n\
out vec4 color;										\n\
													\n\
void main(){										\n\
	color = vCol;									\n\
}													\n\
";


void create_triangle() {
	//Define the points for a triangle
	unsigned int indicies[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2

	};
	GLfloat vertices[] = {	
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f, 1.0f, 0.0f
	};

	//Init VAO and bind it 
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	//Create a Buffer object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
	glBindVertexArray(0);

}

void addShader(GLuint program, const char* shaderCode, GLenum shaderType) {
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

void CompileShaders() {
	shader = glCreateProgram();

	if (!shader) {
		std::cout << "ERROR creating Shader Program" << std::endl;
		//This isn't error safe. make this safer later.
		return;
	}

	addShader(shader, vShader, GL_VERTEX_SHADER);
	std::cout << "Vertex Shader done" << std::endl;
	addShader(shader, fShader, GL_FRAGMENT_SHADER);
	std::cout << "Fragment Shader" << std::endl;

	//Verify the Shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error linking program" << std::endl << eLog << std::endl;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error validating program" << std::endl << eLog << std::endl;
	}

	uniformModel = glGetUniformLocation(shader, "model");
	uniformProjecion = glGetUniformLocation(shader, "projection");
}

int main(int argc, char** argv) {
	//Init Glfw and print an error if it fails. 
	//The tutorial has me use stdio but that isn't smart so I use iostream.
	if (!glfwInit()) {
		std::cout << "GLFW Init Failed" << std::endl;
		glfwTerminate();
		return 1;
	}

	//Setup GLFW window properties
	//OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Get the core profile meaning no backwards compatiablity 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Allow forward compatability. 
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);

	if (!mainWindow) {
		std::cout << "Window creation failed" << std::endl;
		glfwTerminate();
		return 1;
	}

	//Get buffer size information. The dimensions of the window are not the same as the dimension inside the window.
	//Holds all the open GL data inside it.
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	//Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "Glew init failed." << std::endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	//Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	create_triangle();
	CompileShaders();

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth/(GLfloat)bufferHeight, 0.1f, 100.0f);

	//Loop until window closed
	while (!glfwWindowShouldClose(mainWindow)) {
		// Get and handle user input events
		glfwPollEvents();

		if (direction) {
			triOffset += triIncrement;
		}
		else {
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxOffset) {
			direction = !direction;
		}

		currAngle += 0.05f;
		if(currAngle >= 360)
		{
			currAngle -= 360;
		}

		if(sizeDir)
		{
			currSize += 0.0001f;
		}
		else
		{
			currSize -= 0.0001f;
		}

		if(currSize >= maxSize || currSize <= minSize)
		{
			sizeDir = !sizeDir;
		}
		 
		//Clear the window 
		//alpha value determines the opacity higher the more opaque. 
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader);

		//This doen't work the same as in the tutorial. It needs to have a 
		//Specific instructor called in order for the identiy matrix to be created.
		//Thus glm::mat4 model(1.0); and not glm::mat4 model;
		glm::mat4 model(1.0);

		model = glm::translate(model, glm::vec3(0.0f, triOffset, -2.5f));
		model = glm::rotate(model, currAngle * toRadians, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjecion, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glUseProgram(0);

		//Generally you have more than one buffer. one that the user can see
		//The other is the one you are drawing to. 
		//This swaps which one is which allowing for faster loading I think.
		glfwSwapBuffers(mainWindow);
	}
	return 0;
}