#pragma once
#include "GameObjectManager.h"

#include "../myStd/Helper.h"
#include "../myStd/Color.h"
#include <list>
#include <vector>

class GameObjectState
{
public:
	//Transform
	glm::vec2 position;
	glm::vec2 scale;
	float angle;

	//Sprite
	std::string texName;
	Color color;

	//Player
	float speed;

	//Rigidbody
	glm::vec2 velocity;
	float drag;
};