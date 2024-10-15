#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <filesystem>

#include "myStd/Helper.h"

#include "GSM/GameStateManager.h"
#include "GSM/Test.h"
#include "ComponentManager/GameObjectManager.h"

#include "Object/Camera.h"
#include "Editor/MyEditor.h"
#include "source/imgui/imgui.h"
#include "source/imgui/imgui_impl_glfw.h"
#include "source/imgui/imgui_impl_opengl3.h"

void copyAssets()
{
	std::filesystem::path sourceDir = std::filesystem::current_path().parent_path().parent_path() / "Assets\\Shaders";
	std::filesystem::path targetDir = std::filesystem::current_path() / "Assets\\Shaders";

	std::filesystem::create_directories(targetDir);
	for (const auto& entry : std::filesystem::directory_iterator(sourceDir))
		std::filesystem::copy(entry.path(), targetDir / entry.path().filename(), std::filesystem::copy_options::overwrite_existing);	
}

static void init(GLint width, GLint height, std::string title);

void InitImgui(GLFWwindow* window)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();
}
void shutdownImgui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_crtBreakAlloc = 1565;
	//glfwSetErrorCallback(show_glfw_error);
	copyAssets();
	init(1600, 900, "YJEngine");

	//Set Vsync (Buffer swapping, 60fps)
	glfwSwapInterval(0);
	InitImgui(Helper::ptr_window);
	
	Helper::print_specs();

	int nr_extensions = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &nr_extensions);

	GSM::GameStateManager* gsm = GSM::GameStateManager::GetGSMPtr();
	gsm->ChangeLevel(new Levels::Test);

	//Frame Controller
	float frameRate = 1.f / 60.f;
	double lastTime = glfwGetTime();
	double startTime = 0;
	int frames = 0;
	
	std::string str;
	std::string frm = std::to_string(frames) + "fps";

	while(gsm->gGameRunning)
	{
		startTime = glfwGetTime();

		glClear(GL_COLOR_BUFFER_BIT);
		
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow(); // Show demo window! :)
		MyEditor::GetPtr()->Draw();
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
		glfwSetWindowTitle(Helper::ptr_window, str.c_str());

		gsm->Update();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(Helper::ptr_window);

		double curTime = glfwGetTime();
		while (curTime - startTime < frameRate)
			curTime = glfwGetTime();

		if (glfwWindowShouldClose(Helper::ptr_window))
			gsm->gGameRunning = 0;

	}


	gsm->Exit();
	gsm->DeleteGSM();

	GameObjectManager::DeletePtr();

	shutdownImgui();
	MyEditor::DeletePtr();

	glfwDestroyWindow(Helper::ptr_window);
	glfwTerminate();

	return 0;
}

void init(GLint width, GLint height, std::string title)
{
	if (!Helper::init(width, height, title))
	{
		std::cout << "Unable to create OpenGL context" << std::endl;
		std::exit(EXIT_FAILURE);
	}
}