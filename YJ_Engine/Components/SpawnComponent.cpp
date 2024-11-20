#include "SpawnComponent.h"
#include "TimerComponent.h"
#include "PlayerComponent.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "../Object/BombManager.h"


SpawnComponent::SpawnComponent(GameObject* owner) : LogicComponent(owner)
{
	ID = "Spawn";
}

void SpawnComponent::Update()
{
	TimerComponent* timerComponent = (TimerComponent*)owner->FindComponent("Timer");
	if (!timerComponent)
		return;

	if (timerComponent->TimeOut() && spawn == false)
	{
		Draw();
		spawn = true;
	}
}

void SpawnComponent::Draw()
{
	TransformComponent* tComp = (TransformComponent*)owner->FindComponent("Transform");
	glm::vec2 pos = tComp->GetPos();
	if (tComp)
	{
		GameObject* explosion = BombManager::GetPtr()->GetExplosion();
		if (explosion)
		{
			TransformComponent* etComp = (TransformComponent*)explosion->FindComponent("Transform");
			etComp->SetPos(pos);
		}
	}
}

void SpawnComponent::CopyComponent(GameObject* owner)
{
}

void SpawnComponent::LoadFromJson(const json& data)
{
}

json SpawnComponent::SaveToJson()
{
	return json();
}

ComponentSerializer* SpawnComponent::CreateComponent(GameObject* owner)
{
	SpawnComponent* tmp = new SpawnComponent(owner);
	owner->AddComponent(tmp);

	return tmp;
}

