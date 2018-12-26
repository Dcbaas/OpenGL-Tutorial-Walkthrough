#include <iostream>
#include <string.h>
#include <stdio.h>
#include <cmath>
#include <vector>

//include the normal string lib for if it can be used instead
#include <string>
#include <GL/glew.h>
#include <GlfW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.1159265f / 180.0f;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

float currAngle = 0.0f;

bool sizeDir = true;
float currSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

//For the first triangle we are drawing these are global variable
//In future find out how not to use global variables


bool direction = true; 
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

//Create a vertex shader. Nomally this would be done with a file but not for this case. 
static const char* vShader = "Shaders/shader.vert";

//Fragment Shader
static const char* fShader = "Shaders/shader.frag";


void create_objects() {
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

	Mesh* obj1 = new Mesh();
	obj1->createMesh(vertices, indicies, 12, 12);
	meshList.push_back(obj1);

}

void create_shaders()
{
	Shader* shader1 = new Shader();
	shader1->create_from_file(vShader, fShader);
	shaderList.push_back(*shader1);
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

	create_objects();
	create_shaders();

	GLuint uniformProjection = 0, uniformModel = 0; 

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

		shaderList[0].use_shader();
		uniformModel = shaderList[0].get_model_location();
		uniformProjection = shaderList[0].get_projection_location();

		//This doen't work the same as in the tutorial. It needs to have a 
		//Specific instructor called in order for the identiy matrix to be created.
		//Thus glm::mat4 model(1.0); and not glm::mat4 model;
		glm::mat4 model(1.0);

		model = glm::translate(model, glm::vec3(0.0f, triOffset, -2.5f));
		model = glm::rotate(model, currAngle * toRadians, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		meshList[0]->renderMesh();

		glUseProgram(0);

		//Generally you have more than one buffer. one that the user can see
		//The other is the one you are drawing to. 
		//This swaps which one is which allowing for faster loading I think.
		glfwSwapBuffers(mainWindow);
	}
	return 0;
}