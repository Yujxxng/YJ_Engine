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

	using StrObjPair = std::pair<std::string, GameObject*>;
	std::list<StrObjPair> objects;

public:
	static GameObjectManager* GetPtr();
	static void DeletePtr();

	GameObject* GetLastObjects();
	GameObject* FindObjects(std::string);

	void AddObject(GameObject* obj);
	void DeleteObject(GameObject* obj);

	void DeleteAllObject();

	void LoadAllObjects(const json& data);
	void SaveAllObjects(const char* path);
};