#pragma once

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <string>
#include <iostream>

#include "../myStd/Color.h"
#include "Mesh.h"
#include "Shader.h"

typedef struct Texture
{
	const char* texName;
	GLuint tex;
	int width, height;
}Texture;

Texture* CreateTexture(const char* fileName);
