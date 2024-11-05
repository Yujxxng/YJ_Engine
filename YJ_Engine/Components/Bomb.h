#pragma once
#include "../myStd/Helper.h"
#include "../ComponentManager/LogicComponentManager.h"
#include <chrono>

class Bomb : LogicComponent
{
	bool active = false;

	std::chrono::steady_clock::time_point startTime;
	std::chrono::seconds boomTime = std::chrono::seconds(2);

	int power = 1;

public:
	Bomb(GameObject* owner);

	void Update() override;

	void ActiveBomb();

	void LoadFromJson(const json& data) override;
	json SaveToJson() override;

	static ComponentSerializer* CreateComponent(GameObject* owner);
};