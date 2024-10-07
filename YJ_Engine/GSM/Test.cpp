#include "Test.h"
#include "GameStateManager.h"

#include <glew.h>
#include <glfw3.h>

#include <iostream>
#include <string>
#include "../Object/TestObject.h"

TestObject* testobj;
void Levels::Test::Init()
{
	std::cout << "Test Level Init" << std::endl;

	glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	testobj = new TestObject;
	testobj->SetPos(0.f, 0.f);
	testobj->SetSize(0.5f, 0.5f);
	testobj->SetColor(255, 0, 255, 255);
}

void Levels::Test::Update()
{
	

	
	//std::cout << "Test Level Update" << std::endl;
}

void Levels::Test::Exit()
{
	std::cout << "Test Level Exit" << std::endl;
	delete testobj;
}
