#include "Test.h"
#include "GameStateManager.h"

#include <glew.h>
#include <glfw3.h>

#include <iostream>
#include <string>
#include "../Object/Camera.h"
#include "../Object/TestObject.h"

TestObject* testobj;
void Levels::Test::Init()
{
	std::cout << "Test Level Init" << std::endl;
	Camera2D::getPtr()->init(Helper::ptr_window);
	glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	testobj = new TestObject[3];
	testobj[0].SetTexture("Assets/manggom.png");
	testobj[0].SetPos(-300.0f, 0.0f);
	testobj[0].SetSize(210.f, 210.f);
	testobj[0].SetColor(255, 255, 255, 255);
		   
	testobj[1].SetPos(0.0f, 0.0f);
	testobj[1].SetSize(50.f, 50.f);
	testobj[1].SetColor(255, 0, 255, 255);
		   
	testobj[2].SetPos(200.0f, 0.0f);
	testobj[2].SetSize(75.f, 75.f);
	testobj[2].SetColor(0, 0, 255, 255);


}

void Levels::Test::Update()
{
	//std::cout << "Test Level Update" << std::endl;
}

void Levels::Test::Exit()
{
	std::cout << "Test Level Exit" << std::endl;
	
	delete[] testobj;
}
