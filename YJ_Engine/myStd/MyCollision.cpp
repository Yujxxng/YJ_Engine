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

bool CircleAABBCollision(const Circle* c, const AABB* a)
{
    //glm::vec2 q{};
    //ClosestPtPointAABB(c->center, a, q);

    float dist = 0.0f;
    dist = SqDistPointAABB(c->center, a);

    if (dist <= (c->r * c->r)) return true;

    return false;
}

void ClosestPtPointAABB(const glm::vec2 p, AABB* b, glm::vec2& q)
{
    for (int i = 0; i < 2; i++)
    {
        float v = p[i];
        if (v < b->min[i]) v = b->min[i];
        if (v > b->max[i]) v = b->max[i];
        q[i] = v;
    }
}

float SqDistPointAABB(const glm::vec2 p, const AABB* b)
{
    float sqDist = 0.0f;
    for (int i = 0; i < 2; i++)
    {
        float v = p[i];
        if (v < b->min[i]) sqDist += (b->min[i] - v) * (b->min[i] - v);
        if (v > b->max[i]) sqDist += (v - b->max[i]) * (v - b->max[i]);
    }

    return sqDist;
}
