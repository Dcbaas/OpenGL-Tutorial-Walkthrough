#include <iostream>
#include <string.h>

//include the normal string lib for if it can be used instead
#include <string>
#include <GL/glew.h>
#include <GlfW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

//For the first triangle we are drawing these are global variales
//In future find out how not to use global variables
GLuint VAO, VBO, shader; 

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

	//Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	//Loop until window closed
	while (!glfwWindowShouldClose(mainWindow)) {
		// Get and handle user input events
		glfwPollEvents();
		 
		//Clear the window 
		//alpha value determines the opacity higher the more opaque. 
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Generally you have more than one buffer. one that the user can see
		//The other is the one you are drawing to. 
		//This swaps which one is which allowing for faster loading I think.
		glfwSwapBuffers(mainWindow);
	}
	return 0;
}