#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"

#include <algorithm>

bool RigidbodyComponent::CheckEpsilon(float v, float EP)
{
	if (v < -EP || v > EP)
		return true;

	return false;
}

RigidbodyComponent::RigidbodyComponent(GameObject* owner) : EngineComponent(owner), Velocity(), MAXVelocity()
{
	ID = "Rigidbody";

	Velocity.x = 0;
	Velocity.y = 0;
	MAXVelocity.x = 100;
	MAXVelocity.y = 100;
}

glm::vec2 RigidbodyComponent::GetVelocity() const
{
	return Velocity;
}

glm::vec2 RigidbodyComponent::GetMaxVelocity() const
{
	return MAXVelocity;
}

float RigidbodyComponent::GetDrag() const
{
	return drag;
}

void RigidbodyComponent::SetVelocity(const glm::vec2& otherVec)
{
	Velocity = otherVec;

	this->owner->SetDirty(true);
}

void RigidbodyComponent::SetVelocity(float x, float y)
{
	Velocity.x = x;
	Velocity.y = y;

	this->owner->SetDirty(true);
}

void RigidbodyComponent::AddVelocity(const glm::vec2& otherVec)
{
	Velocity.x += otherVec.x;
	Velocity.y += otherVec.y;

	Velocity.x = clamp(Velocity.x, -MAXVelocity.x, MAXVelocity.x);
	Velocity.y = clamp(Velocity.y, -MAXVelocity.y, MAXVelocity.y);

	this->owner->SetDirty(true);
}

void RigidbodyComponent::AddVelocity(float x, float y)
{
	Velocity.x += x;
	Velocity.y += y;

	Velocity.x = clamp(Velocity.x, -MAXVelocity.x, MAXVelocity.x);
	Velocity.y = clamp(Velocity.y, -MAXVelocity.y, MAXVelocity.y);

	this->owner->SetDirty(true);
}

void RigidbodyComponent::ClearVelocity()
{
	Velocity.x = 0;
	Velocity.y = 0;

	this->owner->SetDirty(true);
}

void RigidbodyComponent::SetDrag(float v)
{
	drag = v;

	this->owner->SetDirty(true);
}

void RigidbodyComponent::Update()
{
	TransformComponent* t = (TransformComponent*)owner->FindComponent("Transform");
	if (!t)
		return;

	float x = (float)(t->GetPos().x + Velocity.x * Helper::delta_time); // + 1.0f + 2 * acc * time * time
	float y = (float)(t->GetPos().y + Velocity.y * Helper::delta_time); // + 1.0f + 2 * acc * time * time

	t->SetPos({ x,y });

	//ClearVelocity();

	//If is too low, just set to 0
	if (!CheckEpsilon(Velocity.x))
		Velocity.x = 0;

	if (!CheckEpsilon(Velocity.y))
		Velocity.y = 0;

	ColliderComponent* cComp = (ColliderComponent*)owner->FindComponent("Collider");
	if (!cComp) return;

	x = (float)(cComp->GetPos().x + Velocity.x * Helper::delta_time); // + 1.0f + 2 * acc * time * time
	y = (float)(cComp->GetPos().y + Velocity.y * Helper::delta_time); // + 1.0f + 2 * acc * time * time

	cComp->SetPos(x, y);

}

void RigidbodyComponent::LoadFromJson(const json& data)
{
	auto rigidbodyData = data.find("Rigidbody");
	if (rigidbodyData != data.end())
	{
		Velocity.x = rigidbodyData.value().find("velocity").value().at(0);
		Velocity.y = rigidbodyData.value().find("velocity").value().at(1);

		MAXVelocity.x = rigidbodyData.value().find("maxVelocity").value().at(0);
		MAXVelocity.y = rigidbodyData.value().find("maxVelocity").value().at(1);

		drag = rigidbodyData.value().find("drag").value();
	}
	else
	{
		std::cout << "DATA::EMPTY RIGIDBODY DATA" << std::endl;
		return;
	}
}

json RigidbodyComponent::SaveToJson()
{
	json rigidbody;

	rigidbody["velocity"] = { Velocity.x, Velocity.y };
	rigidbody["maxVelocity"] = { MAXVelocity.x, MAXVelocity.y };
	rigidbody["drag"] = drag;

	return rigidbody;
}

ComponentSerializer* RigidbodyComponent::CreateComponent(GameObject* owner)
{
	RigidbodyComponent* tmp = new RigidbodyComponent(owner);
	owner->AddComponent(tmp);

	return tmp;
}


