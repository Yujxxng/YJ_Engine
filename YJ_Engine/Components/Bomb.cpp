#include "Bomb.h"
#include "SpriteComponent.h"

Bomb::Bomb(GameObject* owner) : LogicComponent(owner)
{
	ID = "Bomb";
}

void Bomb::Update()
{
	if (active)
	{
		auto now = std::chrono::steady_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(now - startTime);
		
		if (elapsedTime >= boomTime)
		{
			std::cout << "Booooom!" << std::endl;
			active = false;
			SpriteComponent* sComp = (SpriteComponent*)owner->FindComponent("Sprite");
			if (!sComp) return;
			sComp->SetColor({ 255, 255, 255, 255 });
		}
	}
}

void Bomb::ActiveBomb()
{
	active = true;
	startTime = std::chrono::steady_clock::now();
}

void Bomb::LoadFromJson(const json& data)
{
}

json Bomb::SaveToJson()
{
	return json();
}

ComponentSerializer* Bomb::CreateComponent(GameObject* owner)
{
	Bomb* tmp = new Bomb(owner);
	owner->AddComponent(tmp);

	return tmp;
}

