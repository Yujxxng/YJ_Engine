#pragma once
#include "../myStd/Helper.h"
#include "../ComponentManager/LogicComponentManager.h"
#include <chrono>

class TimerComponent : LogicComponent
{
	bool active = false;
	bool timeOut = false;
	
	std::chrono::steady_clock::time_point startTime;
	std::chrono::duration<float> timer = std::chrono::duration<float>(2.f);

public:
	TimerComponent(GameObject* owner);

	void Update() override;

	void SetTimer(float second);
	void ActiveTimer();
	bool TimeOut();

	void CopyComponent(GameObject* owner) override;

	void LoadFromJson(const json& data) override;
	json SaveToJson() override;

	static ComponentSerializer* CreateComponent(GameObject* owner);
};