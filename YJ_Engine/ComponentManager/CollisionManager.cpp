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

}
