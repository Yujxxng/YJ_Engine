#pragma once
#include "../myStd/MyFile.h"
#include "../ComponentManager/GameObject.h"
#include "../ComponentManager/GameObjectManager.h"
#include <vector>
#include <memory>

class BombManager
{
	BombManager() = default;
	~BombManager() = default;

	BombManager(const BombManager&) = delete;
	const BombManager& operator=(const BombManager& other) = delete;

	static BombManager* bomb_ptr;

public:
	static BombManager* GetPtr();
	static void DeletePtr();

	int poolSize = 10;
	std::vector<std::pair<GameObject*, int>> BombPool;

	int exPoolSize = 100;
	std::vector<GameObject*> ExplosionPool;

	void InitBombManager();
	void InitBomb(GameObject* bomb);

	GameObject* GetBomb();
	GameObject* GetExplosion();

	void SetBombPower(GameObject* bomb, int n);

	void Explosion(std::pair<GameObject*, int> bomb);

	int GetAliveBomb();

	void Update();
};