#include "BombManager.h"
#include "../ComponentManager/GameObjectManager.h"
#include "../Components/TimerComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/SpawnComponent.h"
#include "../Components/TransformComponent.h"

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
	GameObject* explosion = GameObjectManager::GetPtr()->FindObjects("explosion");

	BombPool.push_back(std::make_pair(bomb, 1));
	ExplosionPool.push_back(explosion);

	for (int i = 0; i < exPoolSize - 1; i++)
	{
		std::string s;
		if (i < poolSize - 1)
		{
			s = "bomb" + std::to_string(i + 1);
			BombPool.push_back(std::make_pair(new GameObject(s), 1));
		}

		s = "explosion" + std::to_string(i + 1);
		ExplosionPool.push_back(new GameObject(s));
	}

	for (int i = 1; i < exPoolSize; i++)
	{
		if(i < poolSize)
			bomb->CopyThisObject(BombPool[i].first);

		explosion->CopyThisObject(ExplosionPool[i]);
	}
}

void BombManager::InitBomb(GameObject* bomb)
{
	bomb->alive = true;
	SpriteComponent* sComp = (SpriteComponent*)bomb->FindComponent("Sprite");
	if (sComp)
		sComp->SetAlpha(255);
	TimerComponent* tComp = (TimerComponent*)bomb->FindComponent("Timer");
	if (tComp)
	{
		tComp->SetTimer(2.0f);
		tComp->ActiveTimer();
	}
	SpawnComponent* spawnComp = (SpawnComponent*)bomb->FindComponent("Spawn");
	if (spawnComp)
	{
		spawnComp->spawn = false;
	}
}


GameObject* BombManager::GetBomb()
{
	for (auto& bomb : BombPool)
	{
		if (!bomb.first->alive)
		{
			InitBomb(bomb.first);
			return bomb.first;
		}
	}
	return nullptr;
}

GameObject* BombManager::GetExplosion()
{
	for (auto& explosion : ExplosionPool)
	{
		if (!explosion->alive)
		{
			explosion->alive = true;
			return explosion;
		}
	}
	return nullptr;
}

void BombManager::SetBombPower(GameObject* bomb, int n)
{
	for (auto& b : BombPool)
	{
		if (b.first == bomb)
			b.second = n;
	}
}

int dx[4] = { -1, 0, 1, 0 };
int dy[4] = { 0, 1, 0, -1 };
void BombManager::Explosion(std::pair<GameObject*, int> bomb)
{
	TransformComponent* tranComp = (TransformComponent*)bomb.first->FindComponent("Transform");
	glm::vec2 pos = tranComp->GetPos();

	std::vector<GameObject*> explosion;
	explosion.reserve(bomb.second * 4 + 1);
	for (int i = 0; i < explosion.size(); i++)
	{
		explosion[i] = GetExplosion();

		TransformComponent* eTranComp = (TransformComponent*)explosion[i]->FindComponent("Transform");
		SpriteComponent* eSpriteComp = (SpriteComponent*)explosion[i]->FindComponent("Sprite");
		if (i == 0)
		{
			eTranComp->SetPos(pos);
			eSpriteComp->SetAlpha(255);
		}
		else
		{
			for (int j = 0; j < 4; j++)
			{
				for (int k = 0; k < bomb.second; k++)
				{
					float x = pos.x + (dx[j] * (k + 1) * (eTranComp->GetScale().x / 2.f));
					float y = pos.y + (dy[j] * (k + 1) * (eTranComp->GetScale().y / 2.f));
					eTranComp->SetPos(x, y);
					eSpriteComp->SetAlpha(255);
				}
			}
		}
		
	}


}

int BombManager::GetAliveBomb()
{
	int num = 0;
	for (auto& bomb : BombPool)
	{
		if (bomb.first->alive)
			num++;
	}

	return num;
}

void BombManager::Update()
{
	for (auto& bomb : BombPool)
	{
		if (bomb.first->alive)
		{
			TimerComponent* tComp = (TimerComponent*)bomb.first->FindComponent("Timer");
			if (!tComp)
				return;

			if(tComp->TimeOut())
			{
				bomb.first->alive = false;

				SpriteComponent* sComp = (SpriteComponent*)bomb.first->FindComponent("Sprite");
				if (sComp)
					sComp->SetAlpha(0);

				Explosion(bomb);
			}
		}
	}
}
