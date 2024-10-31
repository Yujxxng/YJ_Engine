#include "GameObjectManager.h"
#include <fstream>
#include <iostream>

#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/ColliderComponent.h"

#include "../myStd/MyCollision.h"

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

GameObject* GameObjectManager::FindObjects(std::string id)
{
    for (auto& obj : objects)
    {
        if (id == obj.first)
            return obj.second;
    }

    return nullptr;
}

GameObject* GameObjectManager::FindObjects(double x, double y)
{
    for (auto& obj : objects)
    {
        TransformComponent* tComp = (TransformComponent*)obj.second->FindComponent("Transform");
        if (tComp)
        {
            struct AABB tmp = ConvertToAABB(tComp->GetPos(), tComp->GetScale());
            if (PointRectCollision({ x, y }, &tmp))
                return obj.second;
        }
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

void GameObjectManager::ChangeFirstStr(GameObject* obj, std::string newName)
{
    for (auto it = objects.begin(); it != objects.end(); it++)
    {
        if (it->second == obj)
            it->first = newName;
    }
}

void GameObjectManager::DeleteAllObject()
{
    for (auto it = objects.begin(); it != objects.end();)
    {
        delete it->second;
        it = objects.erase(it);
    }

    UpdateIterator();
}

void GameObjectManager::TakeSnapshot()
{
    if(!snapshot.empty())
        snapshot.clear();

    for (const auto& obj : objects)
    {
        if (obj.second->GetDirty())
        {
            GameObjectState state;
            TransformComponent* tComp = (TransformComponent*)obj.second->FindComponent("Transform");
            if (tComp)
            {
                state.position = tComp->GetPos();
                state.scale = tComp->GetScale();
                state.angle = tComp->GetRot();
            }

            SpriteComponent* sComp = (SpriteComponent*)obj.second->FindComponent("Sprite");
            if (sComp)
            {
                state.color = sComp->GetColor();
                state.texName = sComp->GetTexName();
            }

            RigidbodyComponent* rComp = (RigidbodyComponent*)obj.second->FindComponent("Rigidbody");
            if (rComp)
            {
                state.velocity = rComp->GetVelocity();
                state.drag = rComp->GetDrag();
            }

            PlayerComponent* pComp = (PlayerComponent*)obj.second->FindComponent("Player");
            if (pComp)
            {
                state.speed = pComp->GetSpeed();
            }

            ColliderComponent* cComp = (ColliderComponent*)obj.second->FindComponent("Collider");
            if (cComp)
            {
                state.collider_pos = cComp->GetPos();
                state.collider_size = cComp->GetSize();
            }
            snapshot.push_back(std::make_pair(obj.second, state));
            obj.second->ResetDirty();
        }
    }
}

void GameObjectManager::RestoreSnapshot()
{
    for (auto it = snapshot.begin(); it != snapshot.end(); it++)
    {
        TransformComponent* tComp = (TransformComponent*)it->first->FindComponent("Transform");
        if (tComp)
        {
            tComp->SetPos(it->second.position);
            tComp->SetScale(it->second.scale);
            tComp->SetRotate(it->second.angle);
        }

        SpriteComponent* sComp = (SpriteComponent*)it->first->FindComponent("Sprite");
        if (sComp)
        {
            sComp->SetColor(it->second.color);
            //sComp->SetTexture(it->second.texName.c_str());
        }

        RigidbodyComponent* rComp = (RigidbodyComponent*)it->first->FindComponent("Rigidbody");
        if (rComp)
        {
            rComp->SetVelocity(it->second.velocity);
            rComp->SetDrag(it->second.drag);
        }

        PlayerComponent* pComp = (PlayerComponent*)it->first->FindComponent("Player");
        if (pComp)
        {
            pComp->SetSpeed(it->second.speed);
        }

        ColliderComponent* cComp = (ColliderComponent*)it->first->FindComponent("Collider");
        if (cComp)
        {
            cComp->SetPos(it->second.collider_pos.x, it->second.collider_pos.y);
            cComp->SetSize(it->second.collider_size.x, it->second.collider_size.y);
        }
        it->first->ResetDirty();
    }
}

void GameObjectManager::LoadAllObjects(const json& data)
{
    for (auto& obj : data.items())
    {
        std::string id = obj.key();
        GameObject* newObject = new GameObject(id);
        newObject->LoadToJson(data);
    }

    TakeSnapshot();
}

void GameObjectManager::SaveAllObjects(const char* path)
{
    json data;

    for (auto& obj : objects)
        data.merge_patch(obj.second->SaveToJson());

    //std::cout << data << std::endl;

    std::ofstream ofs(path);
    if (!ofs.is_open())
    {
        std::cout << "FILE NOT FOUND" << std::endl;
    }

    ofs << data.dump(4);

    ofs.close();
}
