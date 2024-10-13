#pragma once
#include "GameObject.h"
#include "list"

enum ObjectType
{
	Player = 0,
	Enemy = 1,
};

class GameObjectManager
{
	GameObjectManager() = default;
	~GameObjectManager() = default;
	
	GameObjectManager(const GameObjectManager&) = delete;
	const GameObjectManager& operator=(const GameObjectManager& other) = delete;

	static GameObjectManager* obj_ptr;
	std::list<GameObject*> objects;

public:
	static GameObjectManager* getPtr();
	static void DeletePtr();

	void LoadAllObjects();

	void AddObject(GameObject* obj);
	void DeleteObject(GameObject* obj);
};