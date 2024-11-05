#include "Registry.h"

#include <iostream>
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/SimpleObjectCreator.h"
#include "../Components/PlayerComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/ColliderComponent.h"
#include "../Components/Grid.h"
#include "../Components/Bomb.h"

Registry* Registry::registry_ptr = nullptr;

Registry::Registry()
{
	componentMap.insert({ "Sprite", &SpriteComponent::CreateComponent });
	componentMap.insert({ "SimpleObjectCreator", &SimpleObjectCreator::CreateComponent });
	componentMap.insert({ "Transform", &TransformComponent::CreateComponent });
	componentMap.insert({ "Player", &PlayerComponent::CreateComponent });
	componentMap.insert({ "Rigidbody", &RigidbodyComponent::CreateComponent });
	componentMap.insert({ "Collider", &ColliderComponent::CreateComponent });
	componentMap.insert({ "Grid", &Grid::CreateComponent });
	componentMap.insert({ "Bomb", &Bomb::CreateComponent });
}

ComponentSerializer* Registry::CreateComponent(const std::string& type, GameObject* owner)
{
	if (componentMap.find(type) == componentMap.end())
	{
		std::cerr << "Registry::CreateComponent() Invalid component type " << type << std::endl;
		return nullptr;
	}

	return componentMap[type](owner);
}

Registry* Registry::GetPtr()
{
	if (registry_ptr == nullptr)
	{
		registry_ptr = new Registry;
		return registry_ptr;
	}
	else
		return registry_ptr;
}

void Registry::DeletePtr()
{
	if (registry_ptr != nullptr)
	{
		delete registry_ptr;
		registry_ptr = nullptr;
	}
}
