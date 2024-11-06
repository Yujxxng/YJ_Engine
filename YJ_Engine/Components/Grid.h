#pragma once
#include "../ComponentManager/LogicComponentManager.h"
#include "../myStd/Helper.h"

class Grid : public LogicComponent
{
	int row = 15;
	int col = 13;

	float size = 60.f;
	//900 x 780

	glm::vec2 min{};
	glm::vec2 max{};
public:
	Grid(GameObject* owner);

	void Update() override;

	glm::vec2 GetNearestGridPos(const glm::vec2 playerPos);

	void CopyComponent(GameObject* owner) override;
	
	void LoadFromJson(const json& data) override;
	json SaveToJson() override;

	static ComponentSerializer* CreateComponent(GameObject* owner);
};