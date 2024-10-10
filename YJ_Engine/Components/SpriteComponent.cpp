#include "SpriteComponent.h"

//#include "../ComponentManager/GameObject.h"
//#include "../ComponentManager/EngineComponent.h"

#include "TransformComponent.h"

SpriteComponent::SpriteComponent(GameObject* go) : GraphicComponent(go)
{
	ID = "Sprite";
	shader.setShader("Assets/Shaders/shader.vs", "Assets/Shaders/shader.fs");
	tex = CreateTexture("Assets/white.png");
}

SpriteComponent::~SpriteComponent()
{
	delete tex;
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
#if 0
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
	if (!mTex.empty())
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	else
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	//AEGfxSetColorToMultiply(mColor.r / 255.f, mColor.g / 255.f, mColor.b / 255.f, 255.f);
	if(!mTex.empty())
		AEGfxSetColorToMultiply(1, 1, 1, alpha / 255.f);
	else
		AEGfxSetColorToMultiply(mColor.r / 255.f, mColor.g / 255.f, mColor.b / 255.f, alpha / 255.f);
	
	AEGfxSetColorToAdd(mColor.r / 255.f, mColor.g / 255.f, mColor.b / 255.f, alpha / 255.f);
	
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1);

	if(!mTex.empty())
		AEGfxTextureSet(mTex[index], 0, 0);


	TransformComponent* trs = (TransformComponent*)owner->FindComponent("Transform");
	AEMtx33 tranf = trs->getMatrix();
	AEGfxSetTransform(tranf.m);

	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

	AEGfxMeshFree(mesh);
#elif 1
	shader.use();
	mesh = new Mesh();
	//Color
	mesh->SetColor(color.a, color.g, color.b, color.a);
	
	mesh->SetupMesh();

	//Transform
	TransformComponent* trs = (TransformComponent*)owner->FindComponent("Transform");
	glm::mat3x3 tranf = trs->getMatrix();

	GLint uniform_var_loc1 = glGetUniformLocation(shader.ID, "uModel_to_NDC");
	if (uniform_var_loc1 >= 0) 
		glUniformMatrix3fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(tranf));
	else {
		std::cout << "Uniform variable doesn't exist!!!\n";
		std::exit(EXIT_FAILURE);
	}

	glActiveTexture(GL_TEXTURE0);
	//automatically assign the texture to the fragment shader's sampler
	glBindTexture(GL_TEXTURE_2D, tex->tex);
	mesh->Draw();
	delete mesh;
	shader.unUse();
#endif
}
void SpriteComponent::SetColor(const Color& otherColor)
{
	color = otherColor;
}
void SpriteComponent::SetTexture(const char* fileName)
{
	tex = CreateTexture(fileName);
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
