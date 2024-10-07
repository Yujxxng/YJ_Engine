#include <iostream>
#include "GameObject.h"
#include "EngineComponent.h"
#include "EngineComponentManager.h"

EngineComponent::EngineComponent(GameObject* owner) : BaseComponent(owner)
{
	EngineComponentManager* mgt = EngineComponentManager::getPtr();
	mgt->AddEngine(this);
}

EngineComponent::~EngineComponent()
{
	EngineComponentManager* mgt = EngineComponentManager::getPtr();
	mgt->DeleteEngine(this);
}

GameObject* EngineComponent::GetOwner()
{
	return this->owner;
}

void EngineComponent::Update()
{
	//std::cout << "Engine Update" << std::endl;
}
