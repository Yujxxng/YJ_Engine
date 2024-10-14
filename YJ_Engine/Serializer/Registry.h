#pragma once
#include <string>
#include <map>

class ComponentSerializer;
class GameObject;

// ComponentSerializer* example_func(GameObject* owner){}

class Registry
{
private:
	Registry();
	~Registry() = default;

	Registry(const Registry& other) = delete;
	const Registry& operator= (const Registry& other) = delete;
	
	std::map<std::string, ComponentSerializer* (*)(GameObject* owner)> componentMap;
	static Registry* registry_ptr;


public:
	static Registry* GetPtr();
	static void DeletePtr();

	ComponentSerializer* CreateComponent(const std::string&, GameObject*);
	
	friend class StateSerializer;
};