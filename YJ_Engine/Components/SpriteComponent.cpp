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
	glm::mat3x3 tranf;
	if (trs != nullptr)
		tranf = trs->getMatrix();
	else
		tranf = glm::mat3x3(1.0f);

	GLint uniform_var_color = glGetUniformLocation(shader.ID, "uColor");
	glUniform4f(uniform_var_color, (color.r / 255.f), (color.g / 255.f), (color.b / 255.f), (color.a / 255.f));
	//glUniform2f(glGetUniformLocation(shader.ID, "texSize"), tex->width, tex->height);
	//glUniform3f(uniform_var_color, color.r, color.g, color.b);
	GLint uniform_var_loc1 = glGetUniformLocation(shader.ID, "uModel_to_NDC");
	if (uniform_var_loc1 >= 0) 
		glUniformMatrix3fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(tranf));
	else {
		std::cout << "Uniform variable doesn't exist!!!\n";
	}

	mesh->Draw();
	
	shader.unUse();
}
void SpriteComponent::SetShader(const char* vs, const char* fs)
{
	shader.setShader(vs, fs);
}

void SpriteComponent::SetColor(const Color& otherColor)
{
	color = otherColor;

	this->owner->SetDirty(true);
}
void SpriteComponent::SetColor(float r, float g, float b, float a)
{
	color.r = (unsigned char)r;
	color.g = (unsigned char)g;
	color.b = (unsigned char)b;
	color.a = (unsigned char)a;

	this->owner->SetDirty(true);
}
void SpriteComponent::SetAlpha(float a)
{
	this->color.a = a;
}
void SpriteComponent::SetTexture(const char* fileName)
{
	if (tex)
		delete tex;
	tex = CreateTexture(fileName);

	this->owner->SetDirty(true);
}

Color& SpriteComponent::GetColor()
{
	return this->color;
}

const char* SpriteComponent::GetTexName()
{
	return tex->texName;
}

const char* SpriteComponent::GetFragmentShaderName()
{
	return shader.GetFragmentName();
}

const char* SpriteComponent::GetVertexShaderName()
{
	return shader.GetVertexName();
}

void SpriteComponent::CopyComponent(GameObject* owner)
{
	SpriteComponent* tmp = new SpriteComponent(owner);

	tmp->color = this->color;
	tmp->SetTexture(this->tex->texName);

	owner->AddComponent(tmp);
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

		std::string frag;
		frag = spriteData.value().find("shader").value();
		this->SetShader("Assets/Shaders/shader.vs", frag.c_str());
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
	sprite["shader"] = shader.GetFragmentName();
	//(vtx, fgm) shader_file_name;

	return sprite;
}

ComponentSerializer* SpriteComponent::CreateComponent(GameObject* owner)
{
	SpriteComponent* tmp = new SpriteComponent(owner);
	owner->AddComponent(tmp);
	
	return tmp;
}