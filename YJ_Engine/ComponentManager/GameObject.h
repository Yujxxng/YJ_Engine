#pragma once
#include <map>
#include <list>
#include <string>
#include "BaseComponent.h"

class GameObject
{
	std::string ID;
	std::list<BaseComponent*> components;

public:
	//GameObject();
	~GameObject();

	const std::string GetID() const;
	void SetID(std::string id);

	void AddComponent(BaseComponent* component);
	BaseComponent* FindComponent(std::string cmpID);
	void DeleteComponent(std::string cmpID);
};