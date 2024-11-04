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
	
	for(auto& key : keySet)
	{
		if (key == "LEFT")
		{
			if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT) == GLFW_PRESS)
				rComp->AddVelocity(-speed, 0);
		}

		if(key == "RIGHT")
		{
			if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT) == GLFW_PRESS)
				rComp->AddVelocity(speed, 0);
		}

		if(key == "UP")
		{
			if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP) == GLFW_PRESS)
				rComp->AddVelocity(0, speed);
		}

		if(key == "DOWN")
		{
			if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_DOWN) == GLFW_PRESS)
				rComp->AddVelocity(0, -speed);
		}
		

		if(key == "A")
		{
			if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS)
				rComp->AddVelocity(-speed, 0);
		}

		if (key == "D")
		{
			if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS)
				rComp->AddVelocity(speed, 0);
		}

		if (key == "W")
		{
			if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS)
				rComp->AddVelocity(0, speed);
		}

		if (key == "S")
		{
			if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS)
				rComp->AddVelocity(0, -speed);
		}

		if (key == "LEFT_SHIFT")
		{
			if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
				0;
		}

		if (key == "RIGHT_SHIFT")
		{
			if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
				0;
		}
	}
	/*
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
	*/
}

void PlayerComponent::AddKey(std::string key)
{
	keySet.insert(key);
}

bool PlayerComponent::FindKey(std::string key)
{
	if (keySet.find(key) != keySet.end())
		return true;

	return false;
}

void PlayerComponent::ClearKeySet()
{
	keySet.clear();
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