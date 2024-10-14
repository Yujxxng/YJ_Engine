#pragma once
#include "GameObject.h"
#include "../Object/TestObject.h"
#include "../Serializer/Registry.h"

#include <list>

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
	std::list<std::pair<std::string, GameObject*>> objects;

public:
	static GameObjectManager* getPtr();
	static void DeletePtr();


	void AddObject(GameObject* obj);
	void DeleteObject(GameObject* obj);

	void DeleteAllObject();

	void LoadAllObjects(const json& data);
	void SaveAllObjects();
};