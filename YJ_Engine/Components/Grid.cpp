#include "Grid.h"
#include "TransformComponent.h"

Grid::Grid(GameObject* owner) : LogicComponent(owner)
{
	ID = "Grid";
	TransformComponent* tComp = (TransformComponent*)owner->FindComponent("Transform");
	if (!tComp)
		return;

	glm::vec2 pos = tComp->GetPos();
	glm::vec2 size = tComp->GetScale();

	min.x = pos.x - (size.x / 2.f);
	min.y = pos.y + (size.y / 2.f);

	max.x = pos.x + (size.x / 2.f);
	max.y = pos.y - (size.y / 2.f);

}

void Grid::Update()
{
}

glm::vec2 Grid::GetNearestGridPos(const glm::vec2 playerPos)
{
	glm::vec2 resPos{0, 0};

	resPos.x = ((playerPos.x - min.x) / size) + (size / 2.f);
	resPos.y = ((playerPos.y - min.y) / size) - (size / 2.f);

	return resPos;
}

void Grid::LoadFromJson(const json& data)
{
}

json Grid::SaveToJson()
{
	return json();
}

ComponentSerializer* Grid::CreateComponent(GameObject* owner)
{
	Grid* tmp = new Grid(owner);
	owner->AddComponent(tmp);

	return tmp;
}

