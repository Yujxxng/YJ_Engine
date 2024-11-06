#include "BombManager.h"
#include "../ComponentManager/GameObjectManager.h"
#include "../Components/TimerComponent.h"
#include "../Components/SpriteComponent.h"

#include <string>

BombManager* BombManager::bomb_ptr = nullptr;

BombManager* BombManager::GetPtr()
{
	if (bomb_ptr == nullptr)
	{
		bomb_ptr = new BombManager;
		//bomb_ptr->InitBombManager();

		return bomb_ptr;
	}
	else
		return bomb_ptr;
}

void BombManager::DeletePtr()
{
	if (bomb_ptr != nullptr)
	{
		delete bomb_ptr;
		bomb_ptr = nullptr;
	}
}

void BombManager::InitBombManager()
{
	GameObject* bomb = GameObjectManager::GetPtr()->FindObjects("bomb");

	for (int i = 0; i < poolSize; i++)
		BombPool.push_back(std::make_unique<GameObject>(""));

	for (int i = 0; i < poolSize; i++)
	{
		std::string s = "bomb" + std::to_string(i);

		bomb->CopyThisObject(BombPool[i].get());
		BombPool[i]->SetID(s);
	}
}

GameObject* BombManager::GetBomb()
{
	for (auto& bomb : BombPool)
	{
		if (!bomb->alive)
			return bomb.get();
	}
	return nullptr;
}

int BombManager::GetAliveBomb()
{
	int num = 0;
	for (auto& bomb : BombPool)
	{
		if (bomb->alive)
			num++;
	}

	return num;
}

void BombManager::Update()
{
	for (auto& bomb : BombPool)
	{
		if (bomb->alive)
		{
			SpriteComponent* sComp = (SpriteComponent*)bomb->FindComponent("Sprite");
			sComp->SetAlpha(255);
			TimerComponent* tComp = (TimerComponent*)bomb->FindComponent("Timer");
			tComp->ActiveTimer();
		}
	}
}
