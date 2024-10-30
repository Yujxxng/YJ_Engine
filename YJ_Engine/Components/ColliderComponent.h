#pragma once
#include "../ComponentManager/EngineComponentManager.h"

#include "../myStd/Helper.h"

enum COLLIDER_TYPE
{
	AABB,
	CIRCLE,
};

NLOHMANN_JSON_SERIALIZE_ENUM
(COLLIDER_TYPE,
	{
		{AABB,		"AABB"},
		{CIRCLE,	"CIRCLE"},
	}
	)

enum LAYER : uint32_t
{
	NONE = 0,
	CHARACTER = 1 << 0,
	WALL = 1 << 1,
	BOMB = 1 << 2,
	ITEM = 1 << 3,

	ALL = ~0u
};

class ColliderComponent : public EngineComponent
{
	COLLIDER_TYPE ColliderType = COLLIDER_TYPE::AABB;

	glm::vec2 pos{};
	glm::vec2 size{};
	float radius = -1;

	LAYER layer;
	uint32_t layerMask;

public:
	bool show = false;

	ColliderComponent(GameObject* owner);

	glm::vec2 GetPos() const { return pos; }
	glm::vec2 GetSize() const { return size; }
	COLLIDER_TYPE GetColliderType() const { return ColliderType; }
	float GetRadius() const { return radius; }

	void SetCollision(float posX, float posY, float sizeX, float sizeY);
	void SetPos(float x, float y);
	void SetSize(float x, float y);
	void SetType(COLLIDER_TYPE cType);
	void SetRadius(float r);
	void SetLayer(LAYER l);

	void DrawCollider();

	void Update() override;

	void LoadFromJson(const json&) override;
	json SaveToJson() override;

	static ComponentSerializer* CreateComponent(GameObject* owner);
};