#include <iostream>
#include "GameObject.h"
#include "LogicComponent.h"
#include "LogicComponentManager.h"

LogicComponent::LogicComponent(GameObject* owner) : BaseComponent(owner)
{
	//this->ID = "Logic";

	LogicComponentManager* mgt = LogicComponentManager::getPtr();
	mgt->AddLogic(this);
}

LogicComponent::~LogicComponent()
{
	LogicComponentManager* mgt = LogicComponentManager::getPtr();
	mgt->DeleteLogic(this);
}

GameObject* LogicComponent::GetOwner()
{
	return this->owner;
}

void LogicComponent::Update()
{
	std::cout << "Logic Update" << std::endl;
}
