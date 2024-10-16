#pragma once
#include <map>
#include <list>
#include <vector>
#include <string>
#include "BaseComponent.h"

enum OBJECT_TYPE
{
	TEST = 0,
	CREAT = 1,
};

NLOHMANN_JSON_SERIALIZE_ENUM
(OBJECT_TYPE,
	{
		{TEST, "Test"},
		{CREAT, "Creat"}
	}
)

class GameObject
{
	std::string ID;
	OBJECT_TYPE ObjectType;
	std::list<BaseComponent*> components;

public:
	GameObject() = delete;
	GameObject(std::string id);
	virtual ~GameObject();

	const OBJECT_TYPE GetType() const;
	void SetType(OBJECT_TYPE type);

	const std::string GetID() const;
	void SetID(std::string id);

	void AddComponent(BaseComponent* component);
	void AddComponent(std::string compName);
	BaseComponent* FindComponent(std::string cmpID);
	void DeleteComponent(std::string cmpID);

	bool CheckComponent(std::string compName);
	std::vector<std::string> GetComponentsID();

	void LoadToJson(const json& data);
	json SaveToJson();
};