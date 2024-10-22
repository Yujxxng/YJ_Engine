#pragma once
#include "../ComponentManager/EngineComponentManager.h"

#include "../Utils/myUtils.h"

class ColliderComponent : public EngineComponent
{
	AEVec2 pos{};
	AEVec2 size{};

	bool CheckEpsilon(float v, float EP = EPSILON);

public:
	ColliderComponent(GameObject* owner);

	AEVec2 GetPos() const { return pos; }
	AEVec2 GetSize() const { return size; }

	void SetCollision(float posX, float posY, float sizeX, float sizeY);
	void SetPos(float x, float y);
	bool IsCollision(ColliderComponent* other) const;

	void Update() override;

	void LoadFromJson(const json&) override;
	json SaveToJson() override;

	static ComponentSerializer* CreateComponent(GameObject* owner);
};