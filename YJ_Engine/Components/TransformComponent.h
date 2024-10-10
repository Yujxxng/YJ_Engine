#pragma once
#include "..\ComponentManager\/EngineComponentManager.h"
#include "..\ComponentManager\/EngineComponent.h"

#include <string>
#include <glm.hpp>
#include <glew.h>
#include <glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

class TransformComponent : public EngineComponent
{
#if 0
	glm::vec2 pos{};
	glm::vec2 scale{};
	float rot{};

	glm::mat3 transformMatrix;
	glm::vec2 limit = { 450, 600 }; //window size

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
#elif 1
	float angle_speed, angle_disp;
	
	glm::vec2 position{ 0.0f, 0.0f };
	glm::vec2 scale{ 1.f, 1.f };
	
	glm::mat3 mdl_to_ndc_xform;
	
	void CalculateMatrix();
public:
	TransformComponent(GameObject* go);

	GameObject* GetOwner();

	void Update() override;

	void SetPos(const glm::vec2& otherPos);
	void SetScale(const glm::vec2& otherScale);

	const glm::vec2& GetPos() const { return position; }
	const glm::vec2& GetScale() const { return scale; }
	const float& GetRot() const { return angle_disp; }
	const glm::mat3x3& getMatrix() const { return mdl_to_ndc_xform; }

	//void LoadFromJson(const json&) override;
	//json SaveToJson() override;
	//
	//static ComponentSerializer* CreateComponent(GameObject* owner);
#endif
};
