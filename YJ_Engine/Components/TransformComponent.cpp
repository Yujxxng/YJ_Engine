#include "TransformComponent.h"
#include "../Object/Camera.h"
#include <iostream>
#include <fstream>

#if 0
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
#elif 1
TransformComponent::TransformComponent(GameObject* go) : EngineComponent(go), position(), scale()
{
	ID = "Transform";

	position.x = 0;
	position.y = 0;

	scale.x = 1;
	scale.y = 1;
	CalculateMatrix();
}

void TransformComponent::CalculateMatrix()
{
#if 1
	GLFWwindow* currentWindow = glfwGetCurrentContext();
	GLint wWidth, wHeight;
	glfwGetWindowSize(currentWindow, &wWidth, &wHeight);

	mdl_to_ndc_xform = glm::mat3x3(1.0f);
	glm::mat3 scl_mtx = glm::mat3x3(1.f), rot_mtx = glm::mat3x3(1.f), tra_mtx = glm::mat3x3(1.f);

	scl_mtx = glm::mat3(
		scale.x, 0.f, 0.f,
		0.f, scale.y, 0.f,
		0.f, 0.f, 1.f
	);

	float angle = glm::radians(angle_disp);
	float c = std::cos(angle);
	float s = std::sin(angle);

	rot_mtx = glm::mat3(
		c, s, 0.f,
		-s, c, 0.f,
		0.f, 0.f, 1.f
	);
	//angle_disp += angle_speed * delta_time;

	//position size
	glm::mat3 tra_mtx2 = glm::mat3(
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		position.x, position.y, 1.f
	);
	
	tra_mtx = glm::mat3(
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		position.x, position.y, 1.f
	);

	glm::mat3 h = glm::mat3x3(1.f);
	h = glm::mat3(
		2.0 / wWidth, 0.f, 0.f,
		0.f, 2.0 / wHeight, 0.f,
		0.f, 0.f, 1.f
	);

	glm::mat3 tmp = Camera2D::getPtr()->world_to_ndc_xform;
	mdl_xform = (tra_mtx * rot_mtx * scl_mtx);
	mdl_to_ndc_xform = tmp * mdl_xform;
	
	//mdl_to_ndc_xform = tra_mtx * rot_mtx * scl_mtx * h;

#elif 0
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(position.x, position.y, 0.0f));
	transform = glm::rotate(transform, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::scale(transform, glm::vec3(scale.x, scale.y, 1.0f));

	mdl_to_ndc_xform = transform;

#endif
}
void TransformComponent::Update()
{
	CalculateMatrix();
}

void TransformComponent::SetPos(const glm::vec2& otherPos)
{
	this->position = otherPos;
	CalculateMatrix();
}

void TransformComponent::SetScale(const  glm::vec2& otherScale)
{
	this->scale = otherScale;
	CalculateMatrix();
}

void TransformComponent::LoadFromJson()
{
	std::cout << __FUNCTION__ << std::endl;
	json data;
	data = LoadData(GameDataName);

	if (data == nullptr)
	{
		std::cout << "DATA::EMPTY DATA" << std::endl;
		return;
	}

	for (auto& obj : data.items())
	{
		std::cout << obj.key() << ", " << obj.value() << std::endl;
		if (obj.key() == this->owner->GetID())
		{
			auto compData = obj.value().find("Component Data");
			if (compData != obj.value().end())
			{
				auto tranData = compData.value().find("Transform");
				//std::cout << tranData.key() << " || "  << tranData.value().find("position").value().at(1) << std::endl;
				
				position.x = tranData.value().find("position").value().at(0);
				position.y = tranData.value().find("position").value().at(1);
				
				scale.x = tranData.value().find("scale").value().at(0);
				scale.y = tranData.value().find("scale").value().at(1);
				
				angle_disp = tranData.value().find("rotation").value();
			}
		}
	}
	CalculateMatrix();
}

void TransformComponent::SaveToJson()
{
	std::cout << __FUNCTION__ << std::endl;

	//File open
	std::ifstream ifs(GameDataName);
	if (!ifs.is_open())
	{
		CreateDirectory(GameDataName);
		ifs.open(GameDataName);
	}

	using json = nlohmann::json;
	json data;
	ifs.seekg(0, std::ios::end);
	if (ifs.tellg() != 0) {
		ifs.seekg(0);
		ifs >> data;
	}
	ifs.close();
	/* ---> only save for component
	//Find owner
	for (auto& obj : data.items())
	{
		std::cout << obj.key() << ", " << obj.value() << std::endl;
		if (obj.key() == this->owner->GetID())
		{
			//Get component Data
			auto compData = obj.value().find("Component Data");
			if (compData != obj.value().end())
			{
				json transform;
				transform["position"] = { position.x, position.y };
				transform["scale"] = { scale.x, scale.y };
				transform["rotation"] = angle_disp;

				compData.value()["Transform"] = transform;
			}
		}
	}
	*/
	//Save the data
	json transform, componentData;

	transform["position"] = { position.x, position.y };
	transform["scale"] = { scale.x, scale.y };
	transform["rotation"] = angle_disp;

	componentData["Transform"] = transform;

	std::string ownerName = this->owner->GetID();
	data[ownerName]["Component Data"] = componentData;

	std::ofstream jf(GameDataName);
	jf << data.dump(4);

	jf.close();
}

ComponentSerializer* TransformComponent::CreateComponent(GameObject* owner)
{

	return owner->FindComponent("Transform");
}
#endif

