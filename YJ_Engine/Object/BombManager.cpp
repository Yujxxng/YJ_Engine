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
		bomb_ptr->InitBombManager();

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
	MyFile::LoadObjectFile("prefab.dat");
	GameObject* bomb = GameObjectManager::GetPtr()->FindObjects("bomb");

	BombPool.push_back(bomb);
	for (int i = 0; i < poolSize - 1; i++)
	{
		std::string s = "bomb" + std::to_string(i + 1);
		BombPool.push_back(new GameObject(s));
	}

	for (int i = 1; i < poolSize; i++)
		bomb->CopyThisObject(BombPool[i]);
}

GameObject* BombManager::GetBomb()
{
	for (auto& bomb : BombPool)
	{
		if (!bomb->alive)
			return bomb;
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
			if(sComp)
				sComp->SetAlpha(255);
			TimerComponent* tComp = (TimerComponent*)bomb->FindComponent("Timer");
			if(tComp)
				tComp->ActiveTimer();
		}
	}
}
