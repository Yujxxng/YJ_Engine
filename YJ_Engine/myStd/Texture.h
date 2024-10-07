#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

#include "../myStd/Color.h"
#include "Mesh.h"
#include "Shader.h"

#include <iostream>

class Texture
{
	Mesh* mesh;
	Shader shader;

	GLuint textureName{};

public:
	Texture();
	~Texture();
};