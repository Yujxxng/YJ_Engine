#include "MyCollision.h"

bool AABBCollision(AABB* a, AABB* b)
{
    if (a->max.x < b->min.x || a->min.x > b->max.x) return false;
    if (a->max.y < b->min.y || a->min.y > b->max.y) return false;
    if(a->max.y)
    return true;
}

bool CircleCollision(Circle* a, Circle* b)
{
    glm::vec2 d = a->center - b->center;
    float dist2 = glm::dot(d, d);

    float radiusSum = a->r + b->r;

    return dist2 <= radiusSum * radiusSum;
}

bool PointCircleCollision(const glm::vec2 p, const Circle* c)
{
    float x = c->center.x - p.x;
    float y = c->center.y - p.y;

    float CP = (x * x) + (y * y);

    if (CP <= c->r * c->r)
        return true;

    return false;
}

bool PointRectCollision(const glm::vec2 p, const AABB* a)
{
    if (p.x < a->min.x || p.x > a->max.x ||
        p.y < a->min.y || p.y > a->max.y) return false;

    return true;
}
