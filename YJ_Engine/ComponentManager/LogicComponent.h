#pragma once
#include "BaseComponent.h"
#include <string>

using namespace std;

class LogicComponent : public BaseComponent
{
public:
	LogicComponent() = delete;
	LogicComponent(GameObject* go);
	~LogicComponent();

	GameObject* GetOwner();

	void Update() override;
};
