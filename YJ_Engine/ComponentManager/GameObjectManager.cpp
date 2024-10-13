#include "GameObjectManager.h"

GameObjectManager* GameObjectManager::obj_ptr = nullptr;

GameObjectManager* GameObjectManager::getPtr()
{
    if (obj_ptr == nullptr)
    {
        obj_ptr = new GameObjectManager;

        return obj_ptr;
    }
    else
        return obj_ptr;
}

void GameObjectManager::DeletePtr()
{
    if (obj_ptr != nullptr)
    {
        delete obj_ptr;
        obj_ptr = nullptr;
    }
}

void GameObjectManager::AddObject(GameObject* obj)
{
    objects.push_back(obj);
}

void GameObjectManager::DeleteObject(GameObject* obj)
{
    objects.remove(obj);
}
