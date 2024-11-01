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
	NONE = 1 << 0,
	CHARACTER = 1 << 1,
	ENEMY = 1 << 2,
	WALL = 1 << 3,
	BOMB = 1 << 4,
	ITEM = 1 << 5,

	ALL = CHARACTER | ENEMY | WALL | BOMB | ITEM,
};

NLOHMANN_JSON_SERIALIZE_ENUM
(LAYER,
	{
		{NONE,		"NONE"},
		{CHARACTER,	"CHARACTER"},
		{ENEMY,		"ENEMY"},
		{WALL,		"WALL"},
		{BOMB,		"BOMB"},
		{ITEM,		"ITEM"},
	}
)

class ColliderComponent : public EngineComponent
{
	COLLIDER_TYPE ColliderType = COLLIDER_TYPE::AABB;

	glm::vec2 pos{};
	glm::vec2 size{};
	float radius = -1;

	LAYER layer = NONE;
	uint32_t layerMask = NONE;

public:
	bool show = false;

	ColliderComponent(GameObject* owner);

	glm::vec2 GetPos() const { return pos; }
	glm::vec2 GetSize() const { return size; }
	float GetRadius() const { return radius; }
	COLLIDER_TYPE GetColliderType() const { return ColliderType; }
	LAYER GetLayer() const { return layer; }
	const char* GetLayerToString();
	uint32_t GetLayerMask() const { return layerMask; }

	void SetCollision(float posX, float posY, float sizeX, float sizeY);
	void SetPos(float x, float y);
	void SetSize(float x, float y);
	void SetType(COLLIDER_TYPE cType);
	void SetRadius(float r);
	void SetLayer(enum LAYER l);

	void AddLayerMask(enum LAYER layer);
	void RemoveLayerMask(enum LAYER layer);

	void DrawCollider();

	void Update() override;

	void LoadFromJson(const json&) override;
	json SaveToJson() override;

	static ComponentSerializer* CreateComponent(GameObject* owner);
};