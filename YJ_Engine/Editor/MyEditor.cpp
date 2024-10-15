#include "MyEditor.h"

#include "../GSM/GameStateManager.h"
#include "../ComponentManager/GameObjectManager.h"

#include <iostream>
#include <string>
#include "../myStd/MyFile.h"

MyEditor* MyEditor::editor_ptr = nullptr;


MyEditor* MyEditor::GetPtr()
{
	if (editor_ptr == nullptr)
	{
		editor_ptr = new MyEditor;
		return editor_ptr;
	}
	else
		return editor_ptr;
}

void MyEditor::DeletePtr()
{
	if (editor_ptr != nullptr)
	{
		delete editor_ptr;
		editor_ptr = nullptr;
	}
}

void MyEditor::Draw()
{
	TopBar();
	if (show_new_obj_window)
		ShowCreateNewObjectWindow(&show_new_obj_window);
}

void MyEditor::TopBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ShowMenuFile();
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void MyEditor::ShowMenuFile()
{
	if (ImGui::BeginMenu("New"))
	{
		if (ImGui::MenuItem("File.."))
		{
			std::cout << "File.." << std::endl;
		}
		if (ImGui::BeginMenu("Object.."))
		{
			if (ImGui::MenuItem("TestObject"))
			{
				show_new_obj_window = true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Open"))
	{
		if (ImGui::MenuItem("app.dat"))
			MyFile::LoadObjectFile("app.dat");
		
		ImGui::MenuItem("1");
		ImGui::MenuItem("2");
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Save", "Ctrl_S"))
	{
		std::cout << "Save" << std::endl;
	}
	if (ImGui::MenuItem("Save As.."))
	{
		std::cout << "Save As.." << std::endl;
	}
	if (ImGui::MenuItem("Close", "Alt+F4"))
	{
		std::cout << "Close" << std::endl;
		GSM::GameStateManager::GetGSMPtr()->gGameRunning = 0;
	}
}

void MyEditor::ShowCreateNewObjectWindow(bool* p_open)
{
	ImVec2 windowSize = ImVec2(400, 140);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	float centerX = (windowSize.x - 260.f) * 0.5f;

	if(ImGui::Begin("New Object : TestObject", p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
	{
		float textX = ImGui::CalcTextSize("Enter new object's ID.").x;
		ImGui::SetCursorPosX((windowSize.x - textX) * 0.5f);
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Enter new object's ID.");

		ImGui::NewLine();
		std::string str{};
		ImGui::SetCursorPosX(centerX);
		ImGui::InputTextWithHint("  ", "ID", &str);

		ImGui::NewLine();
		float buttonX = (windowSize.x - 165.f) * 0.5f;
		ImGui::SetCursorPosX(buttonX);
		if (ImGui::Button("Create", ImVec2(80, 0)))
		{
			std::cout << "Create New TestObj" << std::endl;
			show_new_obj_window = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(80, 0)))
		{
			std::cout << "Cancel to create new testobj" << std::endl;
			show_new_obj_window = false;
		}
	}

	ImGui::End();
}
