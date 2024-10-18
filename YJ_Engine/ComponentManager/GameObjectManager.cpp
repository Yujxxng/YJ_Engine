#include "GameObjectManager.h"
#include <fstream>
#include <iostream>

GameObjectManager* GameObjectManager::obj_ptr = nullptr;

GameObjectManager* GameObjectManager::GetPtr()
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

GameObject* GameObjectManager::GetLastObjects()
{
    if (objects.empty()) return nullptr;
    return objects.back().second;
}

GameObject* GameObjectManager::FindObjects(std::string id)
{
    for (auto& obj : objects)
    {
        if (id == obj.first)
            return obj.second;
    }

    return nullptr;
}

int GameObjectManager::GetAllObjectsNumber()
{
    return objects.size();
}

void GameObjectManager::UpdateIterator()
{
    Iter_begin = objects.begin();
    Iter_end = objects.end();
}

void GameObjectManager::AddObject(GameObject* obj)
{
    objects.push_back(std::make_pair(obj->GetID(), obj));
    UpdateIterator();
}

void GameObjectManager::DeleteObject(GameObject* obj)
{
    for (auto it = objects.begin(); it != objects.end();)
    {
        if (it->second == obj)
        {
            delete it->second;
            it = objects.erase(it);
        }
        else
            ++it;
    }
    UpdateIterator();
}

void GameObjectManager::DeleteObject(std::string id)
{
    for (auto it = objects.begin(); it != objects.end();)
    {
        if (it->first == id)
        {
            delete it->second;
            it = objects.erase(it);
        }
        else
            ++it;
    }
    UpdateIterator();
}

void GameObjectManager::DeleteAllObject()
{
    for (auto& obj : objects)
        delete obj.second;
    UpdateIterator();
}

void GameObjectManager::LoadAllObjects(const json& data)
{
    for (auto& obj : data.items())
    {
        std::string id = obj.key();
        GameObject* newObject = new GameObject(id);
        newObject->LoadToJson(data);
    }
}

void GameObjectManager::SaveAllObjects(const char* path)
{
    json data;

    for (auto& obj : objects)
        data.merge_patch(obj.second->SaveToJson());

    std::cout << data << std::endl;

    std::ofstream ofs(path);
    if (!ofs.is_open())
    {
        std::cout << "FILE NOT FOUND" << std::endl;
    }

    ofs << data.dump(4);

    ofs.close();
}
