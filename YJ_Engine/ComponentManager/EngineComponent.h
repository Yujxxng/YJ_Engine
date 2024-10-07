#pragma once
#include "BaseComponent.h"

using namespace std;

class EngineComponent : public BaseComponent
{
public:
	EngineComponent() = delete;
	EngineComponent(GameObject* go);
	~EngineComponent();

	GameObject* GetOwner();

	void Update() override;
};
