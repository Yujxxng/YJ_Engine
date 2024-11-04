#pragma once
#include "../ComponentManager/LogicComponentManager.h"
#include <unordered_set>

class PlayerComponent : public LogicComponent
{
	std::unordered_set<std::string> keySet;
	float speed = 1.f;

public:
	PlayerComponent(GameObject* owner);

	void Update() override;

	void AddKey(std::string key);
	bool FindKey(std::string key);
	void ClearKeySet();
	void SetSpeed(float speed);

	const float GetSpeed() const;
	
	void LoadFromJson(const json& data) override;
	json SaveToJson() override;
	
	static ComponentSerializer* CreateComponent(GameObject* owner);
};