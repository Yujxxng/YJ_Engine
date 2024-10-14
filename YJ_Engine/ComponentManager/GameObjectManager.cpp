#include "GameObjectManager.h"

#include <fstream>
#include <iostream>

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
    objects.push_back(std::make_pair(obj->GetID(), obj));
}

void GameObjectManager::DeleteObject(GameObject* obj)
{
    objects.remove(std::make_pair(obj->GetID(), obj));
}

void GameObjectManager::DeleteAllObject()
{
    for (auto& obj : objects)
        delete obj.second;
}

void GameObjectManager::LoadAllObjects(const json& data)
{
    std::cout << __FUNCTION__ << std::endl;
    std::cout << data << std::endl;

    for (auto& obj : data.items())
    {
        std::string id = obj.key();
        auto tp = obj.value().find("Type").value().get<OBJECT_TYPE>();

        if (tp == TEST)
        {
            TestObject* newObject = new TestObject;
            newObject->SetID(id);
            newObject->LoadToJson(data);

            AddObject(newObject);
        }
    }
}