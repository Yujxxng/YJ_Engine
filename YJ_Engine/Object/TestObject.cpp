#include "TestObject.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/PlayerComponent.h"

TestObject::TestObject(std::string id) : GameObject(id)
{
	SetType(TEST);
	SetPos(0.f, 0.f);
	SetSize(0.5f, 0.5f);
}

TestObject::~TestObject(){}

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
	color.r = (unsigned char)(r / 255.f);
	color.g = (unsigned char)(g / 255.f);
	color.b = (unsigned char)(b / 255.f);
	color.a = (unsigned char)(a / 255.f);
	SpriteComponent* sComp = (SpriteComponent*)FindComponent("Sprite");
	if (sComp != nullptr)
		sComp->SetColor(color);
}

void TestObject::SetTexture(const char* fileName)
{
	SpriteComponent* sComp = (SpriteComponent*)FindComponent("Sprite");
	if (sComp != nullptr)
		sComp->SetTexture(fileName);
}