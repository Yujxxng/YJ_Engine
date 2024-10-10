#pragma once
#include "BaseComponent.h"

using namespace std;

class EngineComponent : public BaseComponent
{
public:
	EngineComponent() = delete;
	EngineComponent(GameObject* go);
	virtual ~EngineComponent() override;

	GameObject* GetOwner();

	void Update() override;
};
