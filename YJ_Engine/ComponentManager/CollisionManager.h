#pragma once
#include <list>

#include "../myStd/MyCollision.h"
#include "../Components/ColliderComponent.h"

class ColliderComponent;

class CollisionManager
{
	CollisionManager() = default;
	~CollisionManager() = default;
	
	CollisionManager(const CollisionManager&) = delete;
	const CollisionManager& operator=(const CollisionManager& other) = delete;

	static CollisionManager* collision_ptr;

	std::list<ColliderComponent*> colliders;

public:
	static CollisionManager* GetPtr();
	static void DeletePtr();

	void Update();

	void AddCollider(ColliderComponent* c);
	bool FindCollider(ColliderComponent* c);
	bool CanCollide(ColliderComponent* a, ColliderComponent* b);

	void SetLayerMask(enum LAYER a, enum LAYER b);
	void RemoveLayerMask(enum LAYER a, enum LAYER b);
};