#include <iostream>
#include "GraphicComponent.h"
#include "GraphicComponentManager.h"

GraphicComponent::GraphicComponent(GameObject* owner) : BaseComponent(owner)
{
	GraphicComponentManager* mgt = GraphicComponentManager::getPtr();
	mgt->AddGraphic(this);
}

GraphicComponent::~GraphicComponent()
{
	GraphicComponentManager* mgt = GraphicComponentManager::getPtr();
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
