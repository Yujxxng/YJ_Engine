#pragma once
#include "../myStd/Helper.h"

struct AABB
{
	glm::vec2 min;
	glm::vec2 max;
};

struct Circle
{
	glm::vec2 center;
	float r;
};

bool AABBCollision(AABB* a, AABB* b);