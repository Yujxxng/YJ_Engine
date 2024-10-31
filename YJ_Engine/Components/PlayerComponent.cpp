#include "PlayerComponent.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"

#include "glew.h"
#include "glfw3.h"

#include <iostream>


PlayerComponent::PlayerComponent(GameObject* owner) : LogicComponent(owner)
{
	ID = "Player";
}

void PlayerComponent::Update()
{
	//TransformComponent* tComp = (TransformComponent*)owner->FindComponent("Transform");
	//if (!tComp)
	//	return;
	
	RigidbodyComponent* rComp = (RigidbodyComponent*)owner->FindComponent("Rigidbody");
	if (!rComp) return;

	//Check for input
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		rComp->AddVelocity(-speed, 0);
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		rComp->AddVelocity(speed, 0);
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP) == GLFW_PRESS)
	{
		rComp->AddVelocity(0, speed);
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		rComp->AddVelocity(0, -speed);
	}
}

void PlayerComponent::SetSpeed(float v)
{
	this->speed = v;

	this->owner->SetDirty(true);
}
const float PlayerComponent::GetSpeed() const
{
	return speed;
}
void PlayerComponent::LoadFromJson(const json& data)
{
	std::cout << __FUNCTION__ << std::endl;

	auto playerData = data.find("Player");
	if (playerData != data.end())
	{
		std::cout << playerData.key() << ", " << playerData.value() << std::endl;
		auto speedData = playerData.value().find("speed");
		speed = speedData.value();
	}
	else
	{
		std::cout << "DATA::EMPTY PLAYER DATA" << std::endl;
		return;
	}

	if (data == nullptr)
	{
		std::cout << "DATA::EMPTY DATA" << std::endl;
		return;
	}
}
json PlayerComponent::SaveToJson()
{
	std::cout << __FUNCTION__ << std::endl;

	json player;

	player["speed"] = speed;

	return player;
}

ComponentSerializer* PlayerComponent::CreateComponent(GameObject* owner)
{
	PlayerComponent* tmp = new PlayerComponent(owner);
	owner->AddComponent(tmp);

	return tmp;
}