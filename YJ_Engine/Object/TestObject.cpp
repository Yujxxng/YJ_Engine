#include "TestObject.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/PlayerComponent.h"

TestObject::TestObject() : GameObject()
{
	//AddComponent(new SpriteComponent(this));
	//AddComponent(new TransformComponent(this));
	//AddComponent(new PlayerComponent(this));

	SetType(TEST);
	SetPos(0.f, 0.f);
	SetSize(0.5f, 0.5f);
}

TestObject::~TestObject()
{
	//DeleteComponent("Sprite");
	//DeleteComponent("Transform");
	//DeleteComponent("Player");
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
/*
void TestObject::LoadToJson()
{
	std::string path = "app.dat";
	std::ifstream jf(path);
	if (!jf.is_open())
	{
		std::ofstream ofs(path);
		ofs.close();
		return;
	}

	using json = nlohmann::json;
	json data;
	jf.seekg(0, std::ios::end);
	if (jf.tellg() != 0) {
		jf.seekg(0);
		jf >> data;
	}
	jf.close();

	for (auto& obj : data.items())
	{
		std::cout << obj.key() << ", " << obj.value() << std::endl;
		if (obj.key() == this->GetID())
		{
			//Get component Data
			auto compData = obj.value().find("Component Data");
			if (compData != obj.value().end())
			{
				SpriteComponent* scompt = (SpriteComponent*)this->FindComponent("Sprite");
				scompt->LoadFromJson(compData.value());

				TransformComponent* tcompt = (TransformComponent*)this->FindComponent("Transform");
				tcompt->LoadFromJson(compData.value());
			}
		}
	}
}
*/

