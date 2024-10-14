#include "SimpleObjectCreator.h"
#include "glew.h"
#include "glfw3.h"

#include <iostream>


SimpleObjectCreator::SimpleObjectCreator(GameObject* owner) : LogicComponent(owner)
{
	ID = "SimpleObjectCreator";
}

void SimpleObjectCreator::Update()
{
}

void SimpleObjectCreator::LoadFromJson(const json& data)
{
	std::cout << __FUNCTION__ << std::endl;
	std::cout << data << std::endl;

	auto creatorData = data.find("SimpleObjectCreator");
	if (creatorData != data.end())
	{
		std::cout << creatorData.key() << ", " << creatorData.value() << std::endl;
		num_column = creatorData.value().find("num_column").value();
		num_row = creatorData.value().find("num_row").value();
	}
	else
	{
		std::cout << "DATA::EMPTY CREATOR DATA" << std::endl;
		return;
	}

	if (data == nullptr)
	{
		std::cout << "DATA::EMPTY DATA" << std::endl;
		return;
	}
}
json SimpleObjectCreator::SaveToJson()
{
	std::cout << __FUNCTION__ << std::endl;

	json creator, componentData;

	creator["num_column"] = num_column;
	creator["num_row"] = num_row;
	//Add Sprite data at Component data
	componentData["SimpleObjectCreator"] = creator;
	return componentData;
}
ComponentSerializer* SimpleObjectCreator::CreateComponent(GameObject* owner)
{
	SimpleObjectCreator* tmp = new SimpleObjectCreator(owner);
	owner->AddComponent(tmp);

	return tmp;
}
