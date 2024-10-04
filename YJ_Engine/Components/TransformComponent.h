#pragma once
#include "..\ComponentManager\/EngineComponentManager.h"
#include "..\ComponentManager\/EngineComponent.h"
#include <string>

using namespace std;

class TransformComponent : public EngineComponent
{
	AEVec2 pos;
	AEVec2 scale;
	float rot;

	AEMtx33 transformMatrix;
	AEVec2 limit = { 450, 600 }; //window size

	void CalculateMatrix();
public:
	TransformComponent(GameObject* go);

	GameObject* GetOwner();

	void Update() override;

	//Getters
	const AEVec2& GetPos() const { return pos; }
	const AEVec2& GetScale() const { return scale; }
	const float& GetRot() const { return rot; }
	const AEMtx33& getMatrix() const { return transformMatrix; }

	//Mutators
	void SetPos(const AEVec2& otherPos);
	void SetScale(const AEVec2& otherScale);
	void SetRot(const float& otherRot);

	void PrintMatrix();

	//void LoadFromJson(const json&) override;
	//json SaveToJson() override;
	//
	//static ComponentSerializer* CreateComponent(GameObject* owner);
};
