#pragma once
#include "GameObject.h"
#include "GameObjectState.h"
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
	std::list<std::pair<GameObject*, GameObjectState>> snapshot;

public:
	static GameObjectManager* GetPtr();
	static void DeletePtr();

	std::list<StrObjPair>::iterator Iter_begin = objects.begin();
	std::list<StrObjPair>::iterator Iter_end = objects.end();
	void UpdateIterator();

	int GetAllObjectsNumber();

	GameObject* FindObjects(std::string);
	GameObject* FindObjects(double x, double y);
	
	void AddObject(GameObject* obj);
	void DeleteObject(GameObject* obj);
	void DeleteObject(std::string id);
	void ChangeFirstStr(GameObject* obj, std::string newName);

	void DeleteAllObject();

	void TakeSnapshot();
	void RestoreSnapshot();

	void LoadAllObjects(const json& data);
	void SaveAllObjects(const char* path);
};