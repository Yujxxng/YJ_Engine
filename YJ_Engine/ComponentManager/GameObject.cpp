#include "GameObject.h"

GameObject::~GameObject()
{
	for (auto it = components.begin(); it != components.end(); it++)
	{
		delete* it;
	}

	components.clear();
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