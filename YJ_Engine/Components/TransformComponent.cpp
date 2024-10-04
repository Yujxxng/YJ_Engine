#include "TransformComponent.h"
#include <iostream>

void TransformComponent::CalculateMatrix()
{
	//Create a transtorm matrix
	AEMtx33 translateMtx;
	AEMtx33Trans(&translateMtx, pos.x, pos.y);
	
	//Create a Rotation matrix
	AEMtx33 rotationMtx;
	AEMtx33Rot(&rotationMtx, rot);

	//Create a scale matrix
	AEMtx33 scaleMtx;
	AEMtx33Scale(&scaleMtx, scale.x, scale.y);

	//Concatenate them
	AEMtx33Concat(&transformMatrix, &rotationMtx, &scaleMtx);
	AEMtx33Concat(&transformMatrix, &translateMtx, &transformMatrix);
}

TransformComponent::TransformComponent(GameObject* go) : EngineComponent(go), pos(), scale(), rot(0), transformMatrix()
{
	ID = "Transform";
	//EngineComponentManager* mgt = EngineComponentManager::getPtr();
	//mgt->AddEngine(this);

	pos.x = 0;
	pos.y = 0;

	scale.x = 1;
	scale.y = 1;

	CalculateMatrix();
}

void TransformComponent::Update()
{
	//PrintMatrix();
	CalculateMatrix();

	float x = AEClamp(pos.x, -limit.x /* + scale.x / 2*/, limit.x);
	float y = AEClamp(pos.y, -limit.y, limit.y);

	SetPos({ x, y });
}

void TransformComponent::SetPos(const AEVec2& otherPos)
{
	this->pos = otherPos;
	CalculateMatrix();
}

void TransformComponent::SetScale(const AEVec2& otherScale)
{
	this->scale = otherScale;
	CalculateMatrix();
}

void TransformComponent::SetRot(const float& otherRot)
{
	this->rot = otherRot;
	CalculateMatrix();
}

void TransformComponent::PrintMatrix()
{
	std::cout << "Printing Transform Comp. With this values : " << std::endl;
	std::cout << "Translate : " << pos.x << " " << pos.y << std::endl;
	std::cout << "   Rotate : " << rot << std::endl;
	std::cout << "    Scale : " << scale.x << " " << scale.y << std::endl;
	std::cout << "-----------------------------------\n";
	for (int i = 0; i < 3; i++)
	{
		std::cout << "|";
		for (int j = 0; j < 3; j++)
			std::cout << " " << transformMatrix.m[i][j];
		std::cout << "|\n";
	}
	std::cout << "-----------------------------------\n";
}

void TransformComponent::LoadFromJson(const json& data)
{
	auto componentData = data.find("componentData");
	if (componentData != data.end())
	{
		auto p = componentData->find("position");
		pos.x = p->begin().value();
		pos.y = (p->begin() + 1).value();

		auto s = componentData->find("scale");
		scale.x = s->begin().value();
		scale.y = (s->begin() + 1).value();

		auto r = componentData->find("rotation");
		rot = r.value();
	}

	CalculateMatrix();
}

json TransformComponent::SaveToJson()
{
	json data, componentData;

	data["type"] = "Transform";

	componentData["position"] = { pos.x, pos.y };
	componentData["scale"] = { scale.x, scale.y };
	componentData["rotation"] = rot;

	data["componentData"] = componentData;

	return data;
}

ComponentSerializer* TransformComponent::CreateComponent(GameObject* owner)
{
	
	return owner->FindComponent("Transform");
}
