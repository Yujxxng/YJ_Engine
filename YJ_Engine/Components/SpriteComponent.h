#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "..\ComponentManager\/GraphicComponent.h"
#include "..\ComponentManager\/GraphicComponentManager.h"
//#include "../ComponentManager/ResourceManager.h"
//#include "../Resource/TextureResource.h"

#include "../myStd/Color.h"
#include "../myStd/Mesh.h"
#include "../myStd/Shader.h"
#include "../myStd/Texture.h"

class SpriteComponent : public GraphicComponent
{
#if 0
private:
	Color mColor{};
	float alpha{ 255.f };

	//AEGfxTexture* mTex = nullptr;
	//std::string texName;

	std::vector<AEGfxTexture*> mTex;
	std::vector<std::string> texName;

	int texNum = 0;
public:
	int index = 0;

public:
	SpriteComponent(GameObject* go);
	~SpriteComponent();

	GameObject* GetOwner();

	void Update() override;

	Color& GetColor() { return mColor; }

	void SetColor(const Color&);
	void SetAlpha(float);
	void SetTexture(std::string s);
	bool FindTextureName(std::string name);

	//void LoadFromJson(const json&) override;
	//json SaveToJson() override;
	//
	//static ComponentSerializer* CreateComponent(GameObject* owner);
#elif 1
private:
	Mesh* mesh;
	Shader shader;

	Color color{255, 255, 255, 255};
	Texture* tex;
	//std::vector<std::string> texture;
	//int texIdx;

public:
	SpriteComponent(GameObject* go);
	~SpriteComponent();

	GameObject* GetOwner();

	void Update() override;

	void SetShader(const char* vs, const char* fs);
	void SetColor(const Color&);
	void SetColor(float r, float g, float b, float a);
	void SetTexture(const char* fileName);

	Color& GetColor();
	const char* GetTexName();
	const char* GetFragmentShaderName();
	const char* GetVertexShaderName();

	void LoadFromJson(const json& data) override;
	json SaveToJson() override;

	static ComponentSerializer* CreateComponent(GameObject* owner);
#endif
};
