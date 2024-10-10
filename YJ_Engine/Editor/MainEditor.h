#pragma once
#include "../source/imgui/imgui.h"
#include "../source/imgui/imgui_impl_glfw.h"
#include "../source/imgui/imgui_impl_opengl3.h"


class MainEditor
{

public:
	void TopBar();
};

void MainEditor::TopBar()
{
	ImGui::BeginMainMenuBar();
	ImGui::Text("This is the main menu %i", 56);
	ImGui::EndMainMenuBar();
}