#pragma once
#include "glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "../myStd/Color.h"

#include "../ComponentManager/GameObject.h"

enum Status
{
	Dead = 0,
	Alive = 1
};

class TestObject : public GameObject
{
	Status status = Dead;
	glm::vec2 size{0.f, 0.f};
	glm::vec2 pos{100.f, 100.f};
	Color color{255, 255, 255, 255};

public:
	TestObject();
	~TestObject();

	void SetSize(float x, float y);
	void SetPos(float x, float y);
	void SetColor(float r, float g, float b, float a);
	void SetTexture(const char* fileName);
};