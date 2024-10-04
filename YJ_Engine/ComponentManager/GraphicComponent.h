#pragma once
#include "BaseComponent.h"

class GraphicComponent : public BaseComponent
{
public:
	GraphicComponent() = delete;
	GraphicComponent(GameObject* go);
	~GraphicComponent();

	GameObject* GetOwner();

	void Update() override;
};
