#include "SpawnComponent.h"
#include "TimerComponent.h"

SpawnComponent::SpawnComponent(GameObject* owner) : LogicComponent(owner)
{
	ID = "Spawn";
}

void SpawnComponent::Update()
{
	TimerComponent* timerComponent = (TimerComponent*)owner->FindComponent("Timer");
	if (!timerComponent)
		return;

	if (timerComponent->TimeOut())
		0; //spawn spray object
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
	return nullptr;
}

