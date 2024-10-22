#include "ColliderComponent.h"
#include "TransformComponent.h"

bool ColliderComponent::CheckEpsilon(float v, float EP)
{
	if (v < -EP || v > EP)
		return true;

	return false;
}

ColliderComponent::ColliderComponent(GameObject* owner) : EngineComponent(owner)
{
	ID = "Collider";

	pos = { 0, 0 };
	size = { 0, 0 };

	TransformComponent* t = (TransformComponent*)this->GetOwner()->FindComponent("Transform");
	if (t != nullptr)
	{
		pos = t->GetPos();
		size = t->GetScale();
	}
}


void ColliderComponent::SetCollision(float posX, float posY, float sizeX, float sizeY)
{
	pos.x = posX;
	pos.y = posY;
	size.x = sizeX;
	size.y = sizeY;
}

void ColliderComponent::SetPos(float x, float y)
{
	pos.x = x;
	pos.y = y;
}

bool ColliderComponent::IsCollision(ColliderComponent* other) const
{
	float my_leftX = pos.x - (size.x / 2.f);
	float my_leftY = pos.y + (size.y / 2.f);
	float my_rightX = pos.x + (size.x / 2.f);
	float my_rightY = pos.y - (size.y / 2.f);

	float other_leftX = other->pos.x - (other->size.x / 2.f);
	float other_leftY = other->pos.y + (other->size.y / 2.f);
	float other_rightX = other->pos.x + (other->size.x / 2.f);
	float other_rightY = other->pos.y - (other->size.y / 2.f);


	if (my_leftX > other_rightX || my_rightX < other_leftX ||
		my_leftY < other_rightY || my_rightY > other_leftY)
		return false;
	
	if (my_leftX <= -(W_WIDTH / 2) || my_rightX >= (W_WIDTH / 2)) return true;

	return true;
}

void ColliderComponent::Update()
{
	TransformComponent* t = (TransformComponent*)this->owner->FindComponent("Transform");
	SetPos(t->GetPos().x, t->GetPos().y);
	/*
	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxVertexList* mesh = AEGfxMeshEnd();

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	//AEGfxSetColorToMultiply(mColor.r / 255.f, mColor.g / 255.f, mColor.b / 255.f, 255.f);
	AEGfxSetColorToMultiply(1.f, 0.f, 0.f, 1.f);

	AEGfxSetColorToAdd(1.f, 0.f, 0.f, 1.f);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1);

	AEMtx33 tranf;
	//Create a transtorm matrix
	AEMtx33 translateMtx;
	AEMtx33Trans(&translateMtx, pos.x, pos.y);

	//Create a Rotation matrix
	AEMtx33 rotationMtx;
	AEMtx33Rot(&rotationMtx, 0);

	//Create a scale matrix
	AEMtx33 scaleMtx;
	AEMtx33Scale(&scaleMtx, size.x, size.y);

	//Concatenate them
	AEMtx33Concat(&tranf, &rotationMtx, &scaleMtx);
	AEMtx33Concat(&tranf, &translateMtx, &tranf);

	AEGfxSetTransform(tranf.m);

	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

	AEGfxMeshFree(mesh);
	*/
}

void ColliderComponent::LoadFromJson(const json& data)
{
	/*
	auto componentData = data.find("componentData");
	if (componentData != data.end())
	{
		auto it = componentData->find("velocity");
		Velocity.x = it->begin().value();
		Velocity.y = (it->begin() + 1).value();

		it = componentData->find("maxVelocity");
		MAXVelocity.x = it->begin().value();
		MAXVelocity.y = (it->begin() + 1).value();

		it = componentData->find("drag");
		drag = it.value();
	}
	*/
}

json ColliderComponent::SaveToJson()
{
	json data, componentData;
	data["type"] = "Collider";

	/*
	componentData["velocity"] = { Velocity.x, Velocity.y };
	componentData["maxVelocity"] = { MAXVelocity.x, MAXVelocity.y };
	componentData["drag"] = drag;
	*/

	data["componentData"] = componentData;
	return data;
}

ComponentSerializer* ColliderComponent::CreateComponent(GameObject* owner)
{
	return owner->FindComponent("Collider");
}


