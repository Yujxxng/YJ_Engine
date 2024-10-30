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

bool AABBCollision(const struct AABB* a, const struct AABB* b);
bool CircleCollision(const struct Circle* a, const struct Circle* b);
bool PointCircleCollision(const glm::vec2 p, const struct Circle* c);
bool PointRectCollision(const glm::vec2 p, const struct AABB* a);
bool CircleAABBCollision(const struct Circle* c, const struct AABB* a);

void ClosestPtPointAABB(const glm::vec2 p, const struct AABB* b, glm::vec2& q);
float SqDistPointAABB(const glm::vec2 p, const struct AABB* b);