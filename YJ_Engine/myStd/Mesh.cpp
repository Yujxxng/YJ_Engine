#include "Mesh.h"


void Mesh::SetupMesh()
{
	primitive_type = GL_TRIANGLES;

	float vertices[] = {
		// positions          // colors						// texture coords
		 0.5f,  0.5f, 1.0f,   color.r, color.g, color.b,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 1.0f,   color.r, color.g, color.b,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 1.0f,   color.r, color.g, color.b,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 1.0f,   color.r, color.g, color.b,   0.0f, 1.0f    // top left 
	};

	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Mesh::SetColor(float r, float g, float b, float a)
{
	color.r = (unsigned char)r / 255.f;
	color.g = (unsigned char)g / 255.f;
	color.b = (unsigned char)b / 255.f;
	color.a = (unsigned char)a / 255.f;
}

void Mesh::Draw()
{
	//setupMesh();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Mesh::Mesh(){}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
