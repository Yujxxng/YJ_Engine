#pragma once
#include "../myStd/Helper.h"
#include "../ComponentManager/LogicComponentManager.h"
#include <chrono>

class SpawnComponent : LogicComponent
{
	GameObject* spawnObj = nullptr;

public:
	bool spawn = false;
	
	SpawnComponent(GameObject* owner);

	void Update() override;

	void Draw();

	void CopyComponent(GameObject* owner) override;

	void LoadFromJson(const json& data) override;
	json SaveToJson() override;

	static ComponentSerializer* CreateComponent(GameObject* owner);
};