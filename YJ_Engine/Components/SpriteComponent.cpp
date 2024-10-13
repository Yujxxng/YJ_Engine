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
		//std::exit(EXIT_FAILURE);
	}

	mesh->Draw();
	
	shader.unUse();
}
void SpriteComponent::SetColor(const Color& otherColor)
{
	color = otherColor;
}
void SpriteComponent::SetTexture(const char* fileName)
{
	if (tex)
		delete tex;
	tex = CreateTexture(fileName);
}

void SpriteComponent::LoadFromJson()
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
				auto spriteData = compData.value().find("Sprite");
				//std::cout << tranData.key() << " || "  << tranData.value().find("position").value().at(1) << std::endl;
				auto colorData = spriteData.value().find("color");
				color.r = colorData.value().at(0);
				color.g = colorData.value().at(1);
				color.b = colorData.value().at(2);
				color.a = colorData.value().at(3);

				std::string texName;
				texName = spriteData.value().find("texName").value();
				this->SetTexture(texName.c_str());
			}
		}
	}
}

void SpriteComponent::SaveToJson()
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
	std::cout << data << std::endl;

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
				json sprite;
				sprite["color"] = { color.r, color.g, color.b, color.a };
				sprite["texName"] = tex->texName;
				//sprite["shader"] = (vtx, fgm) shader_file_name;
				// 
				//Add Sprite data at Component data
				compData.value()["Sprite"] = sprite;
			}
		}
	}
	//Save the data
	std::ofstream jf(GameDataName);
	jf << data.dump(4);

	jf.close();
}

ComponentSerializer* SpriteComponent::CreateComponent(GameObject* owner)
{
	return owner->FindComponent("Sprite");
}
/*
void SpriteComponent::SetColor(const Color& color)
{
	//mColor = color;
}

void SpriteComponent::SetAlpha(float a)
{
	//alpha = a;
}

void SpriteComponent::SetTexture(std::string s)
{
	//if (mTex != nullptr)
	//	AEGfxTextureUnload(mTex);
	//
	//mTex = AEGfxTextureLoad(s.c_str());

	//if (FindTextureName(s))
	//	return;

	//mTex.push_back(ResourceManager::GetPtr()->Get<TextureResource>(s)->GetData());
}

bool SpriteComponent::FindTextureName(std::string name)
{	
#if 0
	if (mTex.empty()) return false;

	for (int i = 0; i < texName.size(); i++)
	{
		if (texName[i] == name)
			return true;
	}

	return false;
#endif
}

void SpriteComponent::LoadFromJson(const json& data)
{
	auto componentData = data.find("componentData");
	if (componentData != data.end())
	{
		auto it = componentData->find("color");
		mColor.r = it->begin().value();
		mColor.g = (it->begin() + 1).value();
		mColor.b = (it->begin() + 2).value();

		//texName = data.at("texName").get<std::string>();
		//SetTexture(texName);
	}
}

json SpriteComponent::SaveToJson()
{
	json data, componentData;
	data["type"] = "Sprite";

	componentData["color"] = { mColor.r, mColor.g, mColor.b };
	//componentData["texName"] = texName;

	data["componentData"] = componentData;

	return data;
}

ComponentSerializer* SpriteComponent::CreateComponent(GameObject* owner)
{
	return owner->FindComponent("Sprite");
}
*/
