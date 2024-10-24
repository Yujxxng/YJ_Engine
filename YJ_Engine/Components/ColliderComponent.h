#pragma once
#include "../ComponentManager/EngineComponentManager.h"

#include "../myStd/Helper.h"

class ColliderComponent : public EngineComponent
{
	glm::vec2 pos{};
	glm::vec2 size{};
public:
	ColliderComponent(GameObject* owner);

	glm::vec2 GetPos() const { return pos; }
	glm::vec2 GetSize() const { return size; }

	void SetCollision(float posX, float posY, float sizeX, float sizeY);
	void SetPos(float x, float y);
	bool IsCollision(ColliderComponent* other) const;

	void Update() override;

	void LoadFromJson(const json&) override;
	json SaveToJson() override;

	static ComponentSerializer* CreateComponent(GameObject* owner);
};