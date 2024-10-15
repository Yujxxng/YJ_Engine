#include "SpriteComponent.h"

//#include "../ComponentManager/GameObject.h"
//#include "../ComponentManager/EngineComponent.h"

#include "TransformComponent.h"

SpriteComponent::SpriteComponent(GameObject* go) : GraphicComponent(go)
{
	ID = "Sprite";

	mesh = new Mesh();
	mesh->SetupMesh();

	shader.setShader("Assets/Shaders/shader.vs", "Assets/Shaders/shader.fs");

	tex = CreateTexture("Assets/white.png");
}

SpriteComponent::~SpriteComponent()
{
	delete tex;
	delete mesh;

	//if (mTex != nullptr && !texName.empty())
	//	ResourceManager::GetPtr()->Unload(texName);
	//if (!mTex.empty() && !texName.empty())
	//{
	//	for(int i = 0; i < texNum; i++)
	//		ResourceManager::GetPtr()->Unload(texName[i]);
	//}

}

void SpriteComponent::Update()
{
	shader.use();
	
	//Color
	mesh->SetColor(color.r, color.g, color.b, color.a);
	
	glActiveTexture(GL_TEXTURE0);
	//automatically assign the texture to the fragment shader's sampler
	glBindTexture(GL_TEXTURE_2D, tex->tex);
	
	//Transform
	TransformComponent* trs = (TransformComponent*)owner->FindComponent("Transform");
	glm::mat3x3 tranf = trs->getMatrix();

	GLint uniform_var_color = glGetUniformLocation(shader.ID, "uColor");
	glUniform3f(uniform_var_color, color.r, color.g, color.b);
	GLint uniform_var_loc1 = glGetUniformLocation(shader.ID, "uModel_to_NDC");
	if (uniform_var_loc1 >= 0) 
		glUniformMatrix3fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(tranf));
	else {
		std::cout << "Uniform variable doesn't exist!!!\n";
	}

	mesh->Draw();
	
	shader.unUse();
}
void SpriteComponent::SetColor(const Color& otherColor)
{
	color = otherColor;
}
void SpriteComponent::SetColor(float r, float g, float b, float a)
{
	color.r = (unsigned char)(r / 255.f);
	color.g = (unsigned char)(g / 255.f);
	color.b = (unsigned char)(b / 255.f);
	color.a = (unsigned char)(a / 255.f);
}
void SpriteComponent::SetTexture(const char* fileName)
{
	if (tex)
		delete tex;
	tex = CreateTexture(fileName);
}

void SpriteComponent::LoadFromJson(const json& data)
{
	std::cout << __FUNCTION__ << std::endl;

	auto spriteData = data.find("Sprite");
	if (spriteData != data.end())
	{
		std::cout << spriteData.key() << ", " << spriteData.value() << std::endl;
		auto colorData = spriteData.value().find("color");
		color.r = colorData.value().at(0);
		color.g = colorData.value().at(1);
		color.b = colorData.value().at(2);
		color.a = colorData.value().at(3);

		std::string texName;
		texName = spriteData.value().find("texName").value();
		this->SetTexture(texName.c_str());
	}
	else
	{
		std::cout << "DATA::EMPTY SPRITE DATA" << std::endl;
		return;
	}

	if (data == nullptr)
	{
		std::cout << "DATA::EMPTY DATA" << std::endl;
		return;
	}
}

json SpriteComponent::SaveToJson()
{
	std::cout << __FUNCTION__ << std::endl;

	json sprite;

	sprite["color"] = { color.r, color.g, color.b, color.a };
	sprite["texName"] = tex->texName;
	//sprite["shader"] = (vtx, fgm) shader_file_name;

	return sprite;
}

ComponentSerializer* SpriteComponent::CreateComponent(GameObject* owner)
{
	SpriteComponent* tmp = new SpriteComponent(owner);
	owner->AddComponent(tmp);
	
	return tmp;
}