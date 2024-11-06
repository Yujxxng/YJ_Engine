#pragma once
#include <map>
#include <list>
#include <vector>
#include <string>
#include "BaseComponent.h"

enum OBJECT_TYPE
{
	PLAYER = 0,
	SCENE = 1,
	OTHERS = 2,
};

NLOHMANN_JSON_SERIALIZE_ENUM
(OBJECT_TYPE,
	{
		{PLAYER,	"PLAYER"},
		{SCENE,		"SCENE"},
		{OTHERS,	"OTHERS"},
	}
)

class GameObject
{
	std::string ID;
	OBJECT_TYPE ObjectType;

	std::list<BaseComponent*> components;

	bool dirty; //If status change

public:
	bool alive = false;
	
	GameObject() = delete;
	GameObject(std::string id);
	virtual ~GameObject();

	const OBJECT_TYPE GetType() const;
	void SetType(OBJECT_TYPE type);

	const std::string GetID() const;
	void SetID(std::string id);

	bool GetDirty() const;
	void SetDirty(bool);
	void ResetDirty();

	void AddComponent(BaseComponent* component);
	void AddComponent(std::string compName);
	BaseComponent* FindComponent(std::string cmpID);
	void DeleteComponent(std::string cmpID);

	bool CheckComponent(std::string compName);
	std::vector<std::string> GetComponentsID();

	void CopyThisObject(GameObject* other);

	void LoadToJson(const json& data);
	json SaveToJson();
};