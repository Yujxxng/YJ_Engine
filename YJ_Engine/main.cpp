#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <filesystem>

#include "GSM/GameStateManager.h"
#include "GSM/Test.h"
#include "ComponentManager/EngineComponentManager.h"
#include "ComponentManager/GraphicComponentManager.h"

void Window_Resized(GLFWwindow* window, int width, int height);

void Key_Pressed(GLFWwindow* window, int key, int scancode, int action, int mods);
void show_glfw_error(int error, const char* description);
void PrintVersionInfo();

void copyAssets()
{
	std::filesystem::path sourceDir = std::filesystem::current_path().parent_path().parent_path() / "Assets\\Shaders";
	std::filesystem::path targetDir = std::filesystem::current_path() / "Assets\\Shaders";

	std::filesystem::create_directories(targetDir);
	for (const auto& entry : std::filesystem::directory_iterator(sourceDir))
		std::filesystem::copy(entry.path(), targetDir / entry.path().filename(), std::filesystem::copy_options::overwrite_existing);	
}

GLuint CreateTexture(const char* fileName)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(fileName, &width, &height, &nrChannels, 0);
	if (data)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
		std::cout << "Failed to load texture" << std::endl;

	stbi_image_free(data);

	return texture;
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	glfwSetErrorCallback(show_glfw_error);
	copyAssets();
	// Initialize the library
	if (!glfwInit()) {
		std::cerr << "fail to init GLFW" << '\n';
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);	

	GLFWwindow* window = glfwCreateWindow(800, 600, "YJ_Engine", NULL, NULL);
	if (!window)
	{
		std::cerr << "fail to genetate window" << '\n';
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, Window_Resized);
	glfwSetKeyCallback(window, Key_Pressed);

	//Set Vsync (Buffer swapping, 60fps)
	glfwSwapInterval(1);

	//Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum errorCode = glewInit();
	if (errorCode != GLEW_OK) {
		std::cerr << "fali to init GLEW " << glewGetErrorString(errorCode) << std::endl;
		glfwTerminate();
		exit(-1);
	}
	PrintVersionInfo();

	int nr_extensions = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &nr_extensions);

	//glClearColor(0, 0, 0, 1);
	//Shader myShader("Assets/Shaders/shader.vs", "Assets/Shaders/shader.fs");
	//
	//GLuint tex = CreateTexture("Assets/manggom.png");
	//glBindTexture(GL_TEXTURE_2D, tex);

	GSM::GameStateManager* gsm = GSM::GameStateManager::GetGSMPtr();
	gsm->ChangeLevel(new Levels::Test);

	double lastTime = glfwGetTime();
	int frames = 0;
	int count = 0;
	
	std::string str;
	std::string frm = std::to_string(frames) + "fps";
	while(gsm->gGameRunning)
	{
		glClear(GL_COLOR_BUFFER_BIT);

		count++;
		double currentTime = glfwGetTime();
		frames++;
		
		if (currentTime - lastTime >= 1.0f)
		{
			//std::cout << 1000.0f / frames << "ms/frame | " << frames << "fps" << std::endl;
			frm = std::to_string(frames) + "fps";
			frames = 0;
			lastTime = currentTime;
		}
		str = "YJ_Engine | " + frm;
		glfwSetWindowTitle(window, str.c_str());

		gsm->Update();

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwWindowShouldClose(window))
			gsm->gGameRunning = 0;
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	gsm->Exit();
	gsm->DeleteGSM();

	return 0;
}

void show_glfw_error(int error, const char* description) {
	std::cerr << "Error: " << description << '\n';
}

void PrintVersionInfo()
{
	std::cout << "OpenGL version : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Vendor : " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer : " << glGetString(GL_RENDERER) << std::endl;
}

void Window_Resized(GLFWwindow* window, int width, int height) {
	std::cout << "Window resized, new window size: " << width << " x " << height << '\n';

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);
}

void Key_Pressed(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == 'Q' && action == GLFW_PRESS) {
		GSM::GameStateManager::GetGSMPtr()->gGameRunning = 0;
	}
}