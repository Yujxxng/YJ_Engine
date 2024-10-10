#pragma once
#include "BaseComponent.h"

class GraphicComponent : public BaseComponent
{
public:
	GraphicComponent() = delete;
	GraphicComponent(GameObject* go);
	virtual ~GraphicComponent() override;

	GameObject* GetOwner();

	void Update() override;
};
