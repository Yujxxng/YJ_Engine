#pragma once
#include "../myStd/Helper.h"
#include "../ComponentManager/LogicComponentManager.h"
#include <chrono>

class SpawnComponent : LogicComponent
{
	bool spawn = false;
	GameObject* spawnObj = nullptr;

public:
	SpawnComponent(GameObject* owner);

	void Update() override;

	void CopyComponent(GameObject* owner) override;

	void LoadFromJson(const json& data) override;
	json SaveToJson() override;

	static ComponentSerializer* CreateComponent(GameObject* owner);
};