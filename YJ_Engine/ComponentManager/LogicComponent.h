#pragma once
#include "BaseComponent.h"
#include <string>

using namespace std;

class LogicComponent : public BaseComponent
{
public:
	LogicComponent() = delete;
	LogicComponent(GameObject* go);
	virtual ~LogicComponent() override;

	GameObject* GetOwner();

	void Update() override;
};
