#pragma once
#include "../myStd/Helper.h"

class Grid
{
	glm::vec2 pos{ 0, 0 };

	int row = 15;
	int col = 13;

	float size = 60.f;
	//900 x 780

public:
	
	void SetPos(float x, float y);

};