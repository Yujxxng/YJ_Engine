#pragma once
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

#include "Shader.h"

class Mesh
{
	GLenum primitive_type;
	GLuint VAO, VBO, EBO;
	GLuint elem_cnt;

public:
	Mesh();
	~Mesh();
	
	void Draw();
};