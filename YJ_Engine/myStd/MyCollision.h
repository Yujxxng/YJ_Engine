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

bool AABBCollision(const AABB* a, const AABB* b);
bool CircleCollision(const Circle* a, const Circle* b);
bool PointCircleCollision(const glm::vec2 p, const Circle* c);
bool PointRectCollision(const glm::vec2 p, const AABB* a);