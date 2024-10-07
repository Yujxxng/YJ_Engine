#pragma once
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

#include "../myStd/Color.h"
#include "Shader.h"

class Mesh
{
	GLenum primitive_type;
	GLuint VAO, VBO, EBO;
	Color color;
public:
	Mesh();
	~Mesh();
	
	void Draw();
	void SetupMesh();
	void SetColor(float r, float g, float b, float a);
};