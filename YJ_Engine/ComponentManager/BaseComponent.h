#pragma once
#include <string>
#include <iostream>

using namespace std;

class GameObject;

class BaseComponent
{
	BaseComponent() = delete;

protected:
	string ID;
	GameObject* owner = nullptr;

public:
	BaseComponent(GameObject* owner) : owner(owner) {}
	BaseComponent(const BaseComponent&) = delete;
	BaseComponent& operator=(const BaseComponent&) = delete;

	//virtual ~BaseComponent() override {}

	string GetID() { return this->ID; }

	virtual void Update() = 0;
};
