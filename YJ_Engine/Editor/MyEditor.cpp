#include "MyEditor.h"

#include <iostream>
#include <vector>
#include <cctype>

#include "../myStd/MyFile.h"
#include "../myStd/Helper.h"
#include "../myStd/Color.h"

#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/PlayerComponent.h"

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

	if (show_delete_obj)
		ShowDeleteObject(&show_delete_obj);


	if (show_add_comp)
		ShowAddComponent(&show_add_comp);

	if (show_delete_cmp)
		ShowDeleteComponent(&show_delete_cmp, tmpStr);

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

void MyEditor::ComponentListBox()
{
	const char* components[] = { "Transform", "Sprite", "Player"};

	if (ImGui::BeginListBox(" ", ImVec2(250.f, 100.f)))
	{
		for (int i = 0; i < IM_ARRAYSIZE(components); i++)
		{
			const bool is_selected = (selected == i);
			if (ImGui::Selectable(components[i], is_selected))
				selected = i;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		if (ImGui::IsMouseDoubleClicked(0))
		{
			if (selected >= 0 && selected < IM_ARRAYSIZE(components))
				selectedObj->AddComponent(components[selected]);
		}
		ImGui::EndListBox();
	}
}

void MyEditor::ShowHasComponent()
{
	std::vector<std::string> compID = selectedObj->GetComponentsID();
	for (int i = 0; i < compID.size(); i++)
	{
		if (ImGui::CollapsingHeader(compID[i].c_str()))
		{
			if (ImGui::TreeNode(compID[i].c_str()))
			{
				DrawComponentProperties(compID[i]);

				ImGui::NewLine();
				ImGui::SetCursorPosX(300.f);
				if (ImGui::Button("Delete", ImVec2(80, 0)))
				{
					tmpStr = compID[i];
					show_delete_cmp = true;
				}

				ImGui::TreePop();
			}
		}
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
	if (ImGui::MenuItem("Save", "Ctrl+S"))
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

void MyEditor::DrawComponentProperties(std::string compName)
{
	if (compName == "Transform")
		DrawTransform();
	else if (compName == "Sprite")
		DrawSprite();
	else if (compName == "Player")
		DrawPlayer();
}

void MyEditor::DrawTransform()
{
	TransformComponent* tComp = (TransformComponent*)selectedObj->FindComponent("Transform");
	if (tComp != nullptr)
	{
		//POSITION
		float pos_x{ tComp->GetPos().x }, pos_y{ tComp->GetPos().y };
		if (ImGui::TreeNode("Position"))
		{
			ImGui::Text("X :"); ImGui::SameLine();
			ImGui::SetNextItemWidth(220.f);
			if (ImGui::SliderFloat("##pos_x", &pos_x, -(Helper::W_WIDTH / 2.f), (Helper::W_WIDTH / 2.f), "%.2f", ImGuiSliderFlags_NoInput))
			{
				pos_x = pos_x;
				tComp->SetPos(pos_x, pos_y);
			}
			ImGui::SameLine(); ImGui::SetNextItemWidth(60.f);
			if (ImGui::InputFloat("##input_pos_x", &pos_x, 0.0f, 0.0f, "%.2f"))
			{
				pos_x = std::max(-(Helper::W_WIDTH / 2.f), std::min(pos_x, (Helper::W_WIDTH / 2.f)));

				pos_x = pos_x;
				tComp->SetPos(pos_x, pos_y);
			}

			ImGui::NewLine();
			ImGui::Text("Y :"); ImGui::SameLine();
			ImGui::SetNextItemWidth(220.f);
			if(ImGui::SliderFloat("##pos_y", &pos_y, -(Helper::W_HEIGHT / 2.f), (Helper::W_HEIGHT / 2.f), "%.2f", ImGuiSliderFlags_NoInput))
			{
				pos_y = pos_y;
				tComp->SetPos(pos_x, pos_y);
			}
			ImGui::SameLine(); ImGui::SetNextItemWidth(60.f);
			if (ImGui::InputFloat("##input_pos_y", &pos_y, 0.0f, 0.0f, "%.2f"))
			{
				pos_y = std::max(-(Helper::W_HEIGHT / 2.f), std::min(pos_y, (Helper::W_HEIGHT / 2.f)));

				pos_y = pos_y;
				tComp->SetPos(pos_x, pos_y);
			}

			ImGui::TreePop();
		}
		
		ImGui::NewLine();
		//SCALE
		float scale_x{ tComp->GetScale().x }, scale_y{ tComp->GetScale().y };
		if (ImGui::TreeNode("Scale"))
		{
			ImGui::Text("X :"); ImGui::SameLine();
			ImGui::SetNextItemWidth(220.f);
			if (ImGui::SliderFloat("##scale_x", &scale_x, 0.f, 1000.f, "%.2f", ImGuiSliderFlags_NoInput))
			{
				scale_x = scale_x;
				tComp->SetScale(scale_x, scale_y);
			}
			ImGui::SameLine(); ImGui::SetNextItemWidth(40.f);
			if (ImGui::InputFloat("##input_scale_x", &scale_x, 0.0f, 0.0f, "%.2f"))
			{
				scale_x = std::max(0.f, std::min(scale_x, 1000.f));

				scale_x = scale_x;
				tComp->SetScale(scale_x, scale_y);
			}

			ImGui::NewLine();
			ImGui::Text("Y :"); ImGui::SameLine();
			ImGui::SetNextItemWidth(220.f);
			if (ImGui::SliderFloat("##scale_y", &scale_y, 0.f, 1000.f, "%.2f", ImGuiSliderFlags_NoInput))
			{
				scale_y = scale_y;
				tComp->SetScale(scale_x, scale_y);
			}
			ImGui::SameLine(); ImGui::SetNextItemWidth(40.f);
			if (ImGui::InputFloat("##input_scale_y", &scale_y, 0.0f, 0.0f, "%.2f"))
			{
				scale_y = std::max(0.f, std::min(scale_y, 1000.f));

				scale_y = scale_y;
				tComp->SetScale(scale_x, scale_y);
			}
			//bool test = false;
			//ImGui::SetCursorPos(ImVec2(365.f, 260.f));
			//ImGui::Checkbox("##uniform", &test);
			ImGui::TreePop();
		}
	
		ImGui::NewLine();
		//ANGLE
		float angle{ tComp->GetRot() };
		if (ImGui::TreeNode("Angle"))
		{
			ImGui::SeparatorText("Degree");
			ImGui::SetNextItemWidth(220.f);
			if (ImGui::SliderFloat("##angle", &angle, 0.f, 360.f, "%.2f", ImGuiSliderFlags_NoInput))
			{
				angle = angle;
				tComp->SetRotate(angle);
			}
			ImGui::SameLine(); ImGui::SetNextItemWidth(60.f);
			if (ImGui::InputFloat("##input_degree", &angle, 0.0f, 0.0f, "%.2f"))
			{
				angle = std::max(0.f, std::min(angle, 360.f));

				angle = angle;
				tComp->SetRotate(angle);
			}
			ImGui::TreePop();
		}
	}
}

void MyEditor::DrawSprite()
{
	SpriteComponent* sComp = (SpriteComponent*)selectedObj->FindComponent("Sprite");
	if (sComp != nullptr)
	{
		//COLOR
		ImVec4 color = ImVec4((float)(sComp->GetColor().r / 255.f), (float)(sComp->GetColor().g / 255.f), (float)(sComp->GetColor().b / 255.f), (float)(sComp->GetColor().a / 255.f));

		if (ImGui::TreeNode("Color"))
		{
			if (ImGui::ColorEdit4("##MyColor2f", (float*)&color))
			{
				color = color;
				sComp->SetColor(color.x * 255.f, color.y * 255.f, color.z * 255.f, color.w * 255.f);
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Image"))
		{
			const char* items[] = { "Default", "Mangom", "Planet" };
			if(ImGui::Combo("##image", &selected_img, items, IM_ARRAYSIZE(items)))
			{
				sComp->SetTexture(SelectImage(selected_img));
			}
			ImGui::TreePop();
		}
	}
}

void MyEditor::DrawPlayer()
{
	PlayerComponent* pComp = (PlayerComponent*)selectedObj->FindComponent("Player");
	if (pComp != nullptr)
	{
		//SPEED
		float speed{ pComp->GetSpeed()};
		if (ImGui::TreeNode("Speed"))
		{
			ImGui::SetNextItemWidth(220.f);
			if (ImGui::SliderFloat("##angle", &speed, -100.f, 100.f, "%.2f", ImGuiSliderFlags_NoInput))
			{
				speed = speed;
				pComp->SetSpeed(speed);
			}
			ImGui::SameLine(); ImGui::SetNextItemWidth(60.f);
			if (ImGui::InputFloat("##input_degree", &speed, 0.0f, 0.0f, "%.2f"))
			{
				speed = std::max(-100.f, std::min(speed, 100.f));

				speed = speed;
				pComp->SetSpeed(speed);
			}
			ImGui::TreePop();
		}
	}
}

const char* MyEditor::SelectImage(int n)
{
	switch (n)
	{
	case 0:
		return "Assets/white.png";
	case 1:
		return "Assets/manggom.png";
	case 2:
		return "Assets/PlanetTexture.png";
	default:
		return "";
	}
}

bool filterInput(const std::string& input) {
	if (input.empty())
		return false;

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
		if (ImGui::InputTextWithHint("  ", "ID", &buf, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CallbackAlways))
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

	if (ImGui::Begin("TestObject", p_open, ImGuiWindowFlags_NoResize))
	{
		ImGui::SetCursorPosX((windowSize.x - 200.f) * 0.5f);
		if (ImGui::Button("Delete Object", ImVec2(200, 0)))
		{
			message = selectedObj->GetID();
			show_delete_obj = true;
		}
		
		ImGui::NewLine();

		ImGui::AlignTextToFramePadding();
		ImGui::Text("ID : ");

		std::string buf{};
		ImGui::SameLine();
		ImGui::SetNextItemWidth(230);
		if (ImGui::InputTextWithHint("  ", selectedObj->GetID().c_str(), &buf, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CallbackAlways))
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
				selectedObj->SetID(tmpStr);
				if(selectedObj->GetID() == tmpStr)
				{
					message = "Successfully modified.";
					alarm_window = true;
				}
			}
		}

		ImGui::NewLine();
		ImGui::SetCursorPosX((windowSize.x - 300.f) * 0.5f);
		if (ImGui::Button("Add Component", ImVec2(300, 0)))
			show_add_comp = true;

		ImGui::NewLine();
		ShowHasComponent();
	}

	ImGui::End();
}

void MyEditor::ShowDeleteObject(bool* p_open)
{
	ImVec2 windowSize = ImVec2(400, 100);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	float centerX = (windowSize.x - 260.f) * 0.5f;

	if (ImGui::Begin(" ", p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
	{
		float textX = ImGui::CalcTextSize("Are you sure you want to delete this object?").x;
		ImGui::SetCursorPosX((windowSize.x - textX) * 0.5f);
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Are you sure you want to delete this object?");

		ImGui::NewLine();
		float buttonX = (windowSize.x - 165.f) * 0.5f;
		ImGui::SetCursorPosX(buttonX);
		if (ImGui::Button("Yes", ImVec2(80, 0)))
		{
			GameObjectManager::GetPtr()->DeleteObject(selectedObj);
			selectedObj = nullptr;
			show_delete_obj = false;

			message = "Successfully deleted";
			alarm_window = true;
			show_obj_setting = false;
			show_add_comp = false;
		}

		ImGui::SameLine();
		if (ImGui::Button("No", ImVec2(80, 0)))
			show_delete_obj = false;
	}

	ImGui::End();
}

void MyEditor::ShowDeleteComponent(bool* p_open, std::string compName)
{
	ImVec2 windowSize = ImVec2(400, 100);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	float centerX = (windowSize.x - 260.f) * 0.5f;

	if (ImGui::Begin(" ", p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
	{
		float textX = ImGui::CalcTextSize("Are you sure you want to delete this component?").x;
		ImGui::SetCursorPosX((windowSize.x - textX) * 0.5f);
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Are you sure you want to delete this component?");

		ImGui::NewLine();
		float buttonX = (windowSize.x - 165.f) * 0.5f;
		ImGui::SetCursorPosX(buttonX);
		if (ImGui::Button("Yes", ImVec2(80, 0)))
		{
			selectedObj->DeleteComponent(tmpStr);
			show_delete_obj = false;

			message = "Successfully deleted";
			alarm_window = true;
			show_delete_cmp = false;
		}

		ImGui::SameLine();
		if (ImGui::Button("No", ImVec2(80, 0)))
			show_delete_cmp = false;
	}

	ImGui::End();
}

void MyEditor::CreateObjChangeWindow(std::string id)
{
	TestObject* testobj = new TestObject(id);
	selectedObj = testobj;

	show_new_obj_window = false;
	show_obj_setting = true;
}

void MyEditor::ShowAddComponent(bool* p_open)
{
	ImVec2 windowSize = ImVec2(300, 200);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

	if (ImGui::Begin("Add Component", p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
	{
		if (ImGui::IsKeyPressed(ImGuiKey_Enter, false))
			show_add_comp = false;

		//Start Listbox
		float textX = ImGui::CalcTextSize("Components").x;
		ImGui::SetCursorPosX((windowSize.x - textX) * 0.5f);
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Components");
		ImGui::NewLine();

		ImGui::SetCursorPos(ImVec2((windowSize.x - 250.f) * 0.5f, 60.f));
		ComponentListBox();

		//Button
		ImGui::SetCursorPos(ImVec2(195, 170));
		if (ImGui::Button("OK", ImVec2(80, 0)))
			show_add_comp = false;
	}
	ImGui::End();
}

void MyEditor::AlarmWindow(bool* p_open, std::string msg)
{
	ImVec2 textSize = ImGui::CalcTextSize(msg.c_str());
	ImVec2 windowSize = ImVec2(300, textSize.y + 80.f);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	if (ImGui::Begin(" ", p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
	{
		//When this window should close
		if (ImGui::IsKeyPressed(ImGuiKey_Enter, false))
			alarm_window = false;
		if (alarm_window && !ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0))
			alarm_window = false;

		ImGui::SetCursorPosX((windowSize.x - textSize.x) * 0.5f);
		ImGui::Text(msg.c_str());

		ImGui::NewLine();
		float buttonX = (windowSize.x - 80.f) * 0.5f;
		ImGui::SetCursorPosX(buttonX);
		if (ImGui::Button("OK", ImVec2(80, 0)))
			alarm_window = false;
	}
	ImGui::End();
}
