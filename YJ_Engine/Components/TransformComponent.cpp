#include "TransformComponent.h"
#include "../Object/Camera.h"
#include <iostream>
#include <fstream>

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

void TransformComponent::LoadFromJson(const json& data)
{
	std::cout << __FUNCTION__ << std::endl;
	std::cout << data << std::endl;

	auto transformData = data.find("Transform");
	if (transformData != data.end())
	{
		std::cout << transformData.key() << ", " << transformData.value() << std::endl;
		position.x = transformData.value().find("position").value().at(0);
		position.y = transformData.value().find("position").value().at(1);

		scale.x = transformData.value().find("scale").value().at(0);
		scale.y = transformData.value().find("scale").value().at(1);

		angle_disp = transformData.value().find("rotation").value();
	}
	else
	{
		std::cout << "DATA::EMPTY TRANSFORM DATA" << std::endl;
		return;
	}
	CalculateMatrix();
}

json TransformComponent::SaveToJson()
{
	std::cout << __FUNCTION__ << std::endl;

	//Save the data
	json transform, componentData;

	transform["position"] = { position.x, position.y };
	transform["scale"] = { scale.x, scale.y };
	transform["rotation"] = angle_disp;

	componentData["Transform"] = transform;
	std::cout << componentData << std::endl;
	return componentData;
}

ComponentSerializer* TransformComponent::CreateComponent(GameObject* owner)
{
	TransformComponent* tmp = new TransformComponent(owner);
	owner->AddComponent(tmp);

	return tmp;
}

