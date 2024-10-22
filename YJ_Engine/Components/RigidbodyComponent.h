#pragma once
#include "../ComponentManager/EngineComponentManager.h"

#include "../myStd/Helper.h"

#define EPSILON 0.00001f

class RigidbodyComponent : public EngineComponent
{
	float drag = 1.5f;
	glm::vec2 Velocity;
	glm::vec2 MAXVelocity;

	bool CheckEpsilon(float v, float EP = EPSILON);

public:
	RigidbodyComponent(GameObject* owner);

	glm::vec2 GetVelocity() const;
	glm::vec2 GetMaxVelocity() const;
	float GetDrag() const;

	void SetVelocity(const glm::vec2& otherVec);
	void SetVelocity(float x, float y);
	void AddVelocity(const glm::vec2& otherVec);
	void AddVelocity(float x, float y);
	void ClearVelocity();
	void SetDrag(float v);

	void Update() override;

	void LoadFromJson(const json&) override;
	json SaveToJson() override;

	static ComponentSerializer* CreateComponent(GameObject* owner);
};