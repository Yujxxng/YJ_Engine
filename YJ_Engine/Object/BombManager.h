#pragma once
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
	std::vector<std::unique_ptr<GameObject>> BombPool;

	void InitBombManager();

	GameObject* GetBomb();
	int GetAliveBomb();

	void Update();
};