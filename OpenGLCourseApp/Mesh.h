#pragma once

#include <GL\glew.h>

class Mesh
{
public:
	Mesh();

	void createMesh(GLfloat * vertices, unsigned * indices, unsigned numVertices, unsigned numIndicies);
	void renderMesh();
	void clearMesh();
	~Mesh();
private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};

