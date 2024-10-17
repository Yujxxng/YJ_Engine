#pragma once
#include "../ComponentManager/LogicComponentManager.h"

class PlayerComponent : public LogicComponent
{
	float speed = 1.f;
public:
	PlayerComponent(GameObject* owner);

	void Update() override;

	void SetSpeed(float speed);

	const float GetSpeed() const;
	void LoadFromJson(const json& data) override;
	json SaveToJson() override;
	//
	static ComponentSerializer* CreateComponent(GameObject* owner);
};