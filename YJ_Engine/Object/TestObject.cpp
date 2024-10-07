#include "TestObject.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/PlayerComponent.h"

TestObject::TestObject() : GameObject()
{
	AddComponent(new SpriteComponent(this));
	AddComponent(new TransformComponent(this));
	AddComponent(new PlayerComponent(this));

	SetPos(0.f, 0.f);
	SetSize(0.5f, 0.5f);
}

TestObject::~TestObject()
{
	DeleteComponent("Sprite");
	DeleteComponent("Transform");
	DeleteComponent("Player");
}

void TestObject::SetSize(float x, float y)
{
	this->size.x = x;
	this->size.y = y;
	TransformComponent* tComp = (TransformComponent*)FindComponent("Transform");
	if (tComp != nullptr)
		tComp->SetScale({ x, y });
}

void TestObject::SetPos(float x, float y)
{
	this->pos.x = x;
	this->pos.y = y;
	TransformComponent* tComp = (TransformComponent*)FindComponent("Transform");
	if (tComp != nullptr)
		tComp->SetPos({ x, y });
}

void TestObject::SetColor(float r, float g, float b, float a)
{
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	SpriteComponent* sComp = (SpriteComponent*)FindComponent("Sprite");
	if (sComp != nullptr)
		sComp->SetColor(color);
}
