#include "Test.h"
#include "GameStateManager.h"

#include <glew.h>
#include <glfw3.h>

#include <iostream>
#include <string>
#include "../Object/Camera.h"
#include "../Object/TestObject.h"

//Camera2D camera2d;
TestObject* testobj;
void Levels::Test::Init()
{
	std::cout << "Test Level Init" << std::endl;

	glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	camera2d.init(Helper::ptr_window);
	testobj = new TestObject[3];
	//testobj->SetTexture("Assets/manggom.png");
	testobj[0].SetPos(-1.0f, 0.0f);
	testobj[0].SetSize(0.5f, 0.5f);
	testobj[0].SetColor(255, 0, 0, 255);
		   
	testobj[1].SetPos(0.0f, 0.0f);
	testobj[1].SetSize(0.1f, 0.5f);
	testobj[1].SetColor(255, 0, 255, 255);
		   
	testobj[2].SetPos(1.0f, 0.0f);
	testobj[2].SetSize(0.5f, 0.5f);
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
