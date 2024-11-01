#include "CollisionManager.h"

CollisionManager* CollisionManager::collision_ptr = nullptr;

CollisionManager* CollisionManager::GetPtr()
{
    if (collision_ptr == nullptr)
    {
        collision_ptr = new CollisionManager;

        return collision_ptr;
    }
    else
        return collision_ptr;
}

void CollisionManager::DeletePtr()
{
    if (collision_ptr != nullptr)
    {
        delete collision_ptr;
        collision_ptr = nullptr;
    }
}

void CollisionManager::Update()
{
    for (auto i = colliders.begin(); i != colliders.end(); i++)
    {
        for (auto j = std::next(i); j != colliders.end(); j++)
        {
            if (CanCollide(*i, *j))
            {
                struct AABB a {}, b{};
                a = ConvertToAABB((*i)->GetPos(), (*i)->GetSize());
                b = ConvertToAABB((*j)->GetPos(), (*j)->GetSize());

                if (AABBCollision(&a, &b))
                    std::cout << "COLLISION!!!!!!!!!!!!!!" << std::endl;
            }
        }
    }
}

void CollisionManager::AddCollider(ColliderComponent* c)
{
    colliders.push_back(c);
}

bool CollisionManager::FindCollider(ColliderComponent* c)
{
    for (auto& collider : colliders)
    {
        if (c == collider)
            return true;
    }

    return false;
}

bool CollisionManager::CanCollide(ColliderComponent* a, ColliderComponent* b)
{
    return (static_cast<uint32_t>(a->GetLayer()) & b->GetLayerMask()) != 0 &&
        (static_cast<uint32_t>(b->GetLayer()) & a->GetLayerMask()) != 0;
}

void CollisionManager::SetLayerMask(enum LAYER a, enum LAYER b)
{
    for (auto& collider : colliders)
    {
        if (collider->GetLayer() == a)
            collider->AddLayerMask(b);
        if (collider->GetLayer() == b)
            collider->AddLayerMask(a);
    }
}
void CollisionManager::RemoveLayerMask(enum LAYER a, enum LAYER b)
{
    for (auto& collider : colliders)
    {
        if (collider->GetLayer() == a)
            collider->RemoveLayerMask(b);
        if (collider->GetLayer() == b)
            collider->RemoveLayerMask(a);
    }
}