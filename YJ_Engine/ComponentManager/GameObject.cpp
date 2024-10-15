#include "GameObject.h"
#include "../Serializer/Registry.h"
#include "../ComponentManager/GameObjectManager.h"

#include <fstream>
#include <iostream>

GameObject::GameObject(std::string id)
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

void GameObject::AddComponent(BaseComponent* component)
{
	this->components.push_back(component);
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

void GameObject::LoadToJson(const json& data)
{
	for (auto& obj : data.items())
	{
		if (obj.key() == this->GetID())
		{
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
