#include "MyEditor.h"

#include "../GSM/GameStateManager.h"
#include "../ComponentManager/GameObjectManager.h"
#include "../Object/TestObject.h"

#include <iostream>
#include <cctype>

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

	if (show_obj_setting)
		ShowTestObjectSetting(&show_obj_setting);

	if (alarm_window)
		AlarmWindow(&alarm_window, message);
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

bool filterInput(const std::string& input) {
	std::string filtered;
	for (char c : input) {
		if (!std::isalnum(c))
			return false;
	}
	return true;
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

		std::string buf{};

		ImGui::NewLine();
		ImGui::SetCursorPosX(centerX);
		if (ImGui::InputTextWithHint("  ", "ID", &buf, ImGuiInputTextFlags_CallbackAlways))
		{
			tmpStr = buf;
			if (ImGui::IsKeyPressed(ImGuiKey_Enter, false))
			{
				if (!filterInput(buf))
				{
					message = "Creation failed. \nOnly letters and numbers are allowed.";
					alarm_window = true;
				}
				else
					CreateObjChangeWindow(buf);
			}
		}

		ImGui::NewLine();
		float buttonX = (windowSize.x - 165.f) * 0.5f;
		ImGui::SetCursorPosX(buttonX);
		if (ImGui::Button("Create", ImVec2(80, 0)))
		{
			if (!filterInput(tmpStr))
			{
				message = "Creation failed. \nOnly letters and numbers are allowed.";
				alarm_window = true;
			}
			else
			{
				CreateObjChangeWindow(tmpStr);
				tmpStr.clear();
			}
		}
		
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(80, 0)))
			show_new_obj_window = false;
	}

	ImGui::End();
}

void MyEditor::ShowTestObjectSetting(bool* p_open)
{
	ImVec2 windowSize = ImVec2(400, 600);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

	TestObject* newObj = (TestObject*)GameObjectManager::GetPtr()->GetLastObjects();
	if (ImGui::Begin("TestObject", p_open, ImGuiWindowFlags_NoResize))
	{
		ImGui::AlignTextToFramePadding();
		ImGui::Text("ID : ");

		std::string buf{};
		ImGui::SameLine();
		ImGui::SetNextItemWidth(230);
		if (ImGui::InputTextWithHint("  ", newObj->GetID().c_str(), &buf, ImGuiInputTextFlags_CallbackAlways))
			tmpStr = buf;

		ImGui::SameLine();
		if (ImGui::Button("Modify", ImVec2(80, 0)))
		{
			if (!filterInput(tmpStr))
			{
				message = "Modification failed. \nOnly letters and numbers are allowed.";
				alarm_window = true;
			}
			else
			{
				newObj->SetID(tmpStr);
				if(newObj->GetID() == tmpStr)
				{
					message = "Successfully modified.";
					alarm_window = true;
				}
			}
		}
	}

	ImGui::End();
}


void MyEditor::CreateObjChangeWindow(std::string id)
{
	TestObject* testobj = new TestObject(id);

	show_new_obj_window = false;
	show_obj_setting = true;
}

void MyEditor::AlarmWindow(bool* p_open, std::string msg)
{
	ImVec2 textSize = ImGui::CalcTextSize(msg.c_str());
	ImVec2 windowSize = ImVec2(300, textSize.y + 80.f);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

	if (ImGui::Begin(" ", p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
	{
		ImGui::SetCursorPosX((windowSize.x - textSize.x) * 0.5f);
		ImGui::Text(msg.c_str());

		ImGui::NewLine();
		float buttonX = (windowSize.x - 80.f) * 0.5f;
		ImGui::SetCursorPosX(buttonX);
		if (ImGui::Button("OK", ImVec2(80, 0)))
		{
			alarm_window = false;
		}
	}
	ImGui::End();
}
