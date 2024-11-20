#pragma once
#include "../ComponentManager/LogicComponentManager.h"
#include "../myStd/Helper.h"
#include <unordered_set>

class PlayerComponent : public LogicComponent
{
	std::unordered_set<std::string> keySet;
	float speed = 1.f;
	bool key_space = false;

	int MaxBombNum = 1;
	int CurBombNum = 0;
	int BombPower = 1;
public:
	PlayerComponent(GameObject* owner);

	void Update() override;

	void AddKey(std::string key);
	bool FindKey(std::string key);
	void ClearKeySet();
	void SetSpeed(float speed);

	const int GetMaxBombNum() const;
	const int GetCurBombNum() const;
	const int GetBombPower() const;

	const float GetSpeed() const;
	glm::vec2 GetNearestGridPos(const glm::vec2 playerPos);

	void SetMaxBombNum(int n);
	void SetCurBombNum(int n);
	void SetBombPower(int n);

	void CopyComponent(GameObject* owner) override;

	void LoadFromJson(const json& data) override;
	json SaveToJson() override;
	
	static ComponentSerializer* CreateComponent(GameObject* owner);
};