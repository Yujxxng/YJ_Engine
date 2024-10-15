#pragma once
#include "../ComponentManager/LogicComponentManager.h"

class SimpleObjectCreator : public LogicComponent
{
	int num_column{};
	int num_row{};

public:
	~SimpleObjectCreator() override;
	SimpleObjectCreator(GameObject* owner);

	void Update() override;

	void LoadFromJson(const json& data) override;
	json SaveToJson() override;
	
	static ComponentSerializer* CreateComponent(GameObject* owner);
};