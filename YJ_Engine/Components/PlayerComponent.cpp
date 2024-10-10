#include "PlayerComponent.h"
#include "TransformComponent.h"
//#include "RigidbodyComponent.h"
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
	
	//RigidbodyComponent* r = (RigidbodyComponent*)owner->FindComponent("Rigidbody");
	//if (!r) return;

	//Check for input
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		//std::cout << "Press LEFT" << std::endl;
		tComp->SetPos({ tComp->GetPos().x - 0.1f, tComp->GetPos().y });
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		//std::cout << "Press RIGHT" << std::endl;
		tComp->SetPos({ tComp->GetPos().x + 0.1f, tComp->GetPos().y });
	}
	//if (AEInputCheckCurr(AEVK_LEFT)) r->AddVelocity(-speed, 0);
	//if (AEInputCheckCurr(AEVK_RIGHT)) r->AddVelocity(speed, 0);
}

void PlayerComponent::SetSpeed(float v)
{
	this->speed = v;
}
/*
void PlayerComponent::LoadFromJson(const json& data)
{
	auto componentData = data.find("componentData");
	if (componentData != data.end())
	{
		auto it = componentData->find("speed");
		speed = it->begin().value();
	}
}

json PlayerComponent::SaveToJson()
{
	json data, componentData;
	data["type"] = "Player";

	componentData["speed"] = speed;
	data["componentData"] = componentData;

	return data;
}

ComponentSerializer* PlayerComponent::CreateComponent(GameObject* owner)
{
	return owner->FindComponent("Player");
}
*/