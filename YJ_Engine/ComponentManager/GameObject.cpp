#include "GameObject.h"
#include "../ComponentManager/GameObjectManager.h"
#include "../Serializer/Registry.h"

#include <fstream>
#include <iostream>

GameObject::GameObject(std::string id) : dirty(false)
{
	ID = id;
	GameObjectManager::GetPtr()->AddObject(this);
}

GameObject::~GameObject()
{
	for (auto it = components.begin(); it != components.end(); it++)
	{
		delete* it;
	}

	components.clear();
}

const OBJECT_TYPE GameObject::GetType() const
{
	return ObjectType;
}

void GameObject::SetType(OBJECT_TYPE type)
{
	this->ObjectType = type;
}

const std::string GameObject::GetID() const
{
	return this->ID;
}

void GameObject::SetID(std::string id)
{
	this->ID = id;
}

bool GameObject::GetDirty() const
{
	return dirty;
}

void GameObject::SetDirty(bool b)
{
	dirty = b;
}

void GameObject::ResetDirty()
{
	dirty = false;
}

void GameObject::AddComponent(BaseComponent* component)
{
	if(!CheckComponent(component->GetID()))
		this->components.push_back(component);
}

void GameObject::AddComponent(std::string compName)
{
	if (!CheckComponent(compName))
		Registry::GetPtr()->CreateComponent(compName, this);
}

BaseComponent* GameObject::FindComponent(std::string cmpID)
{	
	for (auto it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->GetID() == cmpID)
			return (*it);
	}
	return nullptr;
}

void GameObject::DeleteComponent(std::string cmpID)
{
	BaseComponent* tmp = FindComponent(cmpID);
	components.remove(tmp);

	if(tmp)
		delete tmp;
}

bool GameObject::CheckComponent(std::string compName)
{
	for (auto& cmpt : components)
	{
		if (cmpt->GetID() == compName)
			return true;
	}
	return false;
}

std::vector<std::string> GameObject::GetComponentsID()
{
	std::vector<std::string> cID;
	for (auto& c : components)
		cID.push_back(c->GetID());
	
	return cID;
}

void GameObject::CopyThisObject(GameObject* other)
{
	if(other->ID == "")
		other->ID = this->ID + "_copy";
	
	other->ObjectType = this->ObjectType;
	other->alive = this->alive;
	other->dirty = false;

	for (auto& comp : components)
	{
		comp->CopyComponent(other);
	}
}

void GameObject::LoadToJson(const json& data)
{
	for (auto& obj : data.items())
	{
		if (obj.key() == this->GetID())
		{
			//Get Type
			auto obj_type = obj.value().find("Type").value().get<OBJECT_TYPE>();
			SetType(obj_type);

			//Get component Data
			auto compData = obj.value().find("Component Data");
			if (compData != obj.value().end())
			{
				//Add new component
				json compType = compData.value();
				for (auto& comp : compType.items())
					Registry::GetPtr()->CreateComponent(comp.key(), this);

				//input data in components
				for (auto& comp : components)
					comp->LoadFromJson(compData.value());
			}
		}
	}
}

json GameObject::SaveToJson() //need to modify 
{
	std::cout << __FUNCTION__ << std::endl;

	json data, CompData;

	data[ID]["Type"] = ObjectType;
	if(!components.empty())
	{
		for (auto& comp : components)
		{
			CompData[comp->GetID()] = comp->SaveToJson();
		}
	}
	data[ID]["Component Data"] = CompData;
	std::cout << data << std::endl;

	return data;
}
