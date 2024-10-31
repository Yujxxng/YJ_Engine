#pragma once
#include "../ComponentManager/GameObject.h"
#include "../ComponentManager/EngineComponentManager.h"
#include "../ComponentManager/CollisionManager.h"

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

enum LAYER
{
	NONE = 0,
	CHARACTER = 1 << 0,
	WALL = 1 << 1,
	BOMB = 1 << 2,
	ITEM = 1 << 3,

	ALL = CHARACTER | WALL | BOMB | ITEM,
};

class ColliderComponent : public EngineComponent
{
	COLLIDER_TYPE ColliderType = COLLIDER_TYPE::AABB;

	glm::vec2 pos{};
	glm::vec2 size{};
	float radius = -1;

	LAYER layer = NONE;
	uint32_t layerMask;

public:
	bool show = false;

	ColliderComponent(GameObject* owner);

	glm::vec2 GetPos() const { return pos; }
	glm::vec2 GetSize() const { return size; }
	float GetRadius() const { return radius; }
	COLLIDER_TYPE GetColliderType() const { return ColliderType; }
	LAYER GetLayer() const { return layer; }
	const char* GetLayerToString();

	void SetCollision(float posX, float posY, float sizeX, float sizeY);
	void SetPos(float x, float y);
	void SetSize(float x, float y);
	void SetType(COLLIDER_TYPE cType);
	void SetRadius(float r);
	void SetLayer(enum LAYER l);

	void DrawCollider();

	void Update() override;

	void LoadFromJson(const json&) override;
	json SaveToJson() override;

	static ComponentSerializer* CreateComponent(GameObject* owner);
};