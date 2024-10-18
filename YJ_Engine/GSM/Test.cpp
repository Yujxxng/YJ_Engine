#include "Test.h"
#include "GameStateManager.h"

#include <glew.h>
#include <glfw3.h>

#include <fstream>
#include <iostream>
#include <string>
#include "../myStd/MyFile.h"
#include "../Object/Camera.h"
#include "../Object/TestObject.h"

#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"

#include "../ComponentManager/GameObjectManager.h"

void Levels::Test::Init()
{	
	std::cout << "Test Level Init" << std::endl;
	Camera2D::GetPtr()->init(Helper::ptr_window);
	glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Levels::Test::Update()
{
	//std::cout << "Test Level Update" << std::endl;
}

void Levels::Test::Exit()
{
	std::cout << "Test Level Exit" << std::endl;
	GameObjectManager::GetPtr()->DeleteAllObject();
}
