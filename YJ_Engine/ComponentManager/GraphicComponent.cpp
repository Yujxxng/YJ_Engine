#include <iostream>
#include "GraphicComponent.h"
#include "GraphicComponentManager.h"

GraphicComponent::GraphicComponent(GameObject* owner) : BaseComponent(owner)
{
	GraphicComponentManager* mgt = GraphicComponentManager::GetPtr();
	mgt->AddGraphic(this);
}

GraphicComponent::~GraphicComponent()
{
	GraphicComponentManager* mgt = GraphicComponentManager::GetPtr();
	mgt->DeleteGraphic(this);
}

GameObject* GraphicComponent::GetOwner()
{
	return this->owner;
}

void GraphicComponent::Update()
{
	std::cout << "Graphic Update" << std::endl;
}
