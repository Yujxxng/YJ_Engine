#include "PlayerComponent.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include "SpriteComponent.h"
#include "Grid.h"
#include "TimerComponent.h"
#include "../Object/BombManager.h"

#include "glew.h"
#include "glfw3.h"

#include <iostream>


PlayerComponent::PlayerComponent(GameObject* owner) : LogicComponent(owner)
{
	ID = "Player";
}

void PlayerComponent::Update()
{
	TransformComponent* tComp = (TransformComponent*)owner->FindComponent("Transform");
	if (!tComp)
		return;
	
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

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		glm::vec2 tmp = GetNearestGridPos(tComp->GetPos());
		GameObject* bomb = GameObjectManager::GetPtr()->FindObjects("bomb");
		
		TransformComponent* bomb_tComp = (TransformComponent*)bomb->FindComponent("Transform");
		bomb_tComp->SetPos(tmp);
		
		SpriteComponent* sComp = (SpriteComponent*)bomb->FindComponent("Sprite");
		sComp->SetAlpha(255);
		
		TimerComponent* timerComp = (TimerComponent*)bomb->FindComponent("Timer");
		timerComp->ActiveTimer();
		
		//bomb->alive = true;

		//std::cout << tComp->GetPos().x << ", " << tComp->GetPos().y << " || " << tmp.x << ", " << tmp.y << std::endl;
		/*TimerComponent* timer = (TimerComponent*)owner->FindComponent("Timer");

		if (!timer) return;
		timer->ActiveTimer();

		SpriteComponent* sComp = (SpriteComponent*)owner->FindComponent("Sprite");
		if (!sComp) return;
		sComp->SetColor({ 255, 0, 0, 255 });*/
	}
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

glm::vec2 PlayerComponent::GetNearestGridPos(const glm::vec2 playerPos) //need to modi
{
	glm::vec2 resPos{ 0, 0 };

	//resPos.x = ((playerPos.x - min.x) / size) + (size / 2.f);
	//resPos.y = ((playerPos.y - min.y) / size) - (size / 2.f);
	glm::vec2 min{};
	glm::vec2 max{};
	glm::vec2 size{ 900.f, 780.f };
	min.x = 0.f - (size.x / 2.f);
	min.y = 0.f + (size.y / 2.f);

	max.x = 0.f + (size.x / 2.f);
	max.y = 0.f - (size.y / 2.f);

	glm::ivec2 idx{};
	idx.x = glm::abs((playerPos.x - min.x) / 60.f);
	idx.y = glm::abs((playerPos.y - min.y) / 60.f);

	resPos.x = min.x + (60.f * idx.x) + 30.f;
	resPos.y = min.y - (60.f * idx.y) - 30.f;

	return resPos;
}
void PlayerComponent::CopyComponent(GameObject* owner)
{
	PlayerComponent* tmp = new PlayerComponent(owner);
	if (!keySet.empty())
	{
		for (auto& k : keySet)
			tmp->keySet.insert(k);
	}
	tmp->speed = this->speed;

	owner->AddComponent(tmp);
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