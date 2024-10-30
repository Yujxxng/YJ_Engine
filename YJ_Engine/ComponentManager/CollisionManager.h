#pragma once
#include "GameObject.h"
#include "../Components/ColliderComponent.h"
#include "../myStd/MyCollision.h"

class CollisionManager
{
	CollisionManager() = default;
	~CollisionManager() = default;
	
	CollisionManager(const CollisionManager&) = delete;
	const CollisionManager& operator=(const CollisionManager& other) = delete;

	static CollisionManager* collision_ptr;

public:
	static CollisionManager* GetPtr();
	static void DeletePtr();

	void Update();
};