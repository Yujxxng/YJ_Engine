#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "../myStd/Mesh.h"
#include "../myStd/Shader.h"
#include "../Object/Camera.h"
#include "../myStd/MyCollision.h"

ColliderComponent::ColliderComponent(GameObject* owner) : EngineComponent(owner)
{
	ID = "Collider";

	ColliderType = COLLIDER_TYPE::AABB;
	pos = { 0, 0 };
	size = { 0, 0 };

	TransformComponent* t = (TransformComponent*)this->GetOwner()->FindComponent("Transform");
	if (t != nullptr)
	{
		pos = t->GetPos();
		size = t->GetScale();
	}
}


const char* ColliderComponent::GetLayerToString()
{
	switch (this->layer)
	{
	case NONE:
		return "None";

	case CHARACTER:
		return "Character";

	case ENEMY:
		return "Enemy";

	case WALL:
		return "Wall";

	case BOMB:
		return "Bomb";

	case ITEM:
		return "Item";

	default:
		return "";
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

	pos.x = std::min(std::max(pos.x, -Helper::W_WIDTH / 2.f), (Helper::W_WIDTH / 2.f));
	pos.y = std::min(std::max(pos.y, -Helper::W_HEIGHT / 2.f), (Helper::W_HEIGHT / 2.f));
}

void ColliderComponent::SetSize(float x, float y)
{
	size.x = x;
	size.y = y;
}

void ColliderComponent::SetType(COLLIDER_TYPE cType)
{
	ColliderType = cType;
}

void ColliderComponent::SetRadius(float r)
{
	radius = r;
}

void ColliderComponent::SetLayer(enum LAYER l)
{
	layer = l;
	
	if(!CollisionManager::GetPtr()->FindCollider(this))
		CollisionManager::GetPtr()->AddCollider(this);
}

void ColliderComponent::AddLayerMask(enum LAYER layer)
{
	this->layerMask |= static_cast<uint32_t>(layer);
}

void ColliderComponent::RemoveLayerMask(enum LAYER layer)
{
	this->layerMask &= ~static_cast<uint32_t>(layer);
}

void ColliderComponent::DrawCollider()
{
	Mesh* mesh = new Mesh;
	mesh->SetupMesh();

	Shader shader;
	shader.setShader("Assets/Shaders/shader.vs", "Assets/Shaders/shader.fs");

	shader.use();

	//Color
	mesh->SetColor(255, 0, 0, 255);

	//Transform
	glm::mat3 scl_mtx = glm::mat3x3(1.f), rot_mtx = glm::mat3x3(1.f), tra_mtx = glm::mat3x3(1.f);

	scl_mtx = glm::mat3(
		size.x, 0.f, 0.f,
		0.f, size.y, 0.f,
		0.f, 0.f, 1.f
	);

	float angle = glm::radians(0.0f);
	float c = std::cos(angle);
	float s = std::sin(angle);

	rot_mtx = glm::mat3(
		c, s, 0.f,
		-s, c, 0.f,
		0.f, 0.f, 1.f
	);

	tra_mtx = glm::mat3(
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		pos.x, pos.y, 1.f
	);

	glm::mat3 tmp = Camera2D::GetPtr()->world_to_ndc_xform;
	glm::mat3x3 tranf = tra_mtx * rot_mtx * scl_mtx;
	tranf = tmp * tranf;

	GLint uniform_var_color = glGetUniformLocation(shader.ID, "uColor");
	glUniform3f(uniform_var_color, (255.f / 255.f), (0.f / 255.f), (0.f / 255.f));
	//glUniform3f(uniform_var_color, color.r, color.g, color.b);
	GLint uniform_var_loc1 = glGetUniformLocation(shader.ID, "uModel_to_NDC");
	if (uniform_var_loc1 >= 0)
		glUniformMatrix3fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(tranf));
	else {
		std::cout << "Uniform variable doesn't exist!!!\n";
	}

	mesh->Draw();

	shader.unUse();
	delete mesh;
}

void ColliderComponent::Update()
{/*
	TransformComponent* t = (TransformComponent*)this->owner->FindComponent("Transform");
	if(t)
		SetPos(t->GetPos().x, t->GetPos().y);*/
	/*
	double cursorX, cursorY;
	glfwGetCursorPos(Helper::ptr_window, &cursorX, &cursorY);
	cursorX = cursorX - (Helper::W_WIDTH / 2.f);
	cursorY = (Helper::W_HEIGHT / 2.f) - cursorY;
	std::cout << cursorX << ", " << cursorY << std::endl;

	AABB test;
	test.max.x = pos.x + (size.x / 2.f);
	test.max.y = pos.y + (size.y / 2.f);
	test.min.x = pos.x - (size.x / 2.f);
	test.min.y = pos.y - (size.x / 2.f);

	if (PointRectCollision({ cursorX, cursorY }, &test))
		std::cout << "collision" << std::endl;
	*/

	if(show)
		DrawCollider();
}

void ColliderComponent::CopyComponent(GameObject* owner)
{
	ColliderComponent* tmp = new ColliderComponent(owner);
	tmp->pos = this->pos;
	tmp->size = this->size;
	tmp->radius = this->radius;
	tmp->layer = this->layer;
	tmp->layerMask = this->layerMask;
	tmp->show = this->show;

	owner->AddComponent(tmp);
}

void ColliderComponent::LoadFromJson(const json& data)
{
	auto colliderData = data.find("Collider");
	if (colliderData != data.end())
	{
		std::cout << colliderData.key() << ", " << colliderData.value() << std::endl;
		auto collider_type = colliderData.value().find("Type").value().get<COLLIDER_TYPE>();
		ColliderType = collider_type;

		pos.x = colliderData.value().find("position").value().at(0);
		pos.y = colliderData.value().find("position").value().at(1);

		size.x = colliderData.value().find("scale").value().at(0);
		size.y = colliderData.value().find("scale").value().at(1);
	}
	else
	{
		std::cout << "DATA::EMPTY COLLIDER DATA" << std::endl;
		return;
	}
}

json ColliderComponent::SaveToJson()
{
	json collider;
	collider["Type"] = ColliderType;
	collider["position"] = { pos.x, pos.y };
	collider["scale"] = { size.x, size.y };

	return collider;
}

ComponentSerializer* ColliderComponent::CreateComponent(GameObject* owner)
{
	ColliderComponent* tmp = new ColliderComponent(owner);
	owner->AddComponent(tmp);

	return tmp;
}


