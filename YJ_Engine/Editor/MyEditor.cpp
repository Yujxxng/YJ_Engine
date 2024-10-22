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
#include "../Components/RigidbodyComponent.h"

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
	if (show_all_obj)
		ShowAllObjects(&show_all_obj);

	if (show_new_obj_window)
		ShowCreateNewObjectWindow(&show_new_obj_window);

	if (show_obj_setting)
		ShowObjectSetting(&show_obj_setting);
	else
		show_add_comp = false;

	if (show_delete_obj)
		ShowDeleteObject(&show_delete_obj);

	if (show_add_comp)
		ShowAddComponent(&show_add_comp);

	if (show_delete_cmp)
		ShowDeleteComponent(&show_delete_cmp, tmpStr);

	if (show_make_new_file)
		ShowMakeNewFile(&show_make_new_file);

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

		ImGui::SetCursorPosX(1520.f);

		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(4.f / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(4.f / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(4.f / 7.0f, 0.8f, 0.8f));

		if (ImGui::Button("Save", ImVec2(40.f, 18.f)))
		{
			SaveFile();
		}

		ImGui::PopStyleColor(3);
		ImGui::PopID();

		ImGui::SetCursorPosX(1560.f);

		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));

		if (ImGui::Button("Exit", ImVec2(40.f, 18.f)))
		{
			GameObjectManager::GetPtr()->TakeSnapshot();
			Helper::editMode = false;
		}
	
		ImGui::PopStyleColor(3);
		ImGui::PopID();
		ImGui::EndMainMenuBar();
	}
}

void MyEditor::ComponentListBox()
{
	const char* components[] = { "Transform", "Sprite", "Player", "Rigidbody" };

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
		if (ImGui::MenuItem("Object.."))
		{
			show_new_obj_window = true;
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Open"))
	{
		const char* paths[] = {
			"app.dat",
			"yrds.dat"
		};

		for (auto path : paths)
		{
			if (ImGui::MenuItem(path))
			{
				if (!curPath.empty())
					CloseFile();

				curPath = path;
				MyFile::LoadObjectFile(path);
			}
		}
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Save", "Ctrl+S"))
	{
		std::cout << "Save" << std::endl;
		SaveFile();
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
	else if (compName == "Rigidbody")
		DrawRigidbody();
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
			const char* items[] = { "Assets/white.png", "Assets/manggom.png", "Assets/PlanetTexture.png" };
			const char* combo_preview = sComp->GetTexName();
			for (int i = 0; i < IM_ARRAYSIZE(items); i++)
			{
				if (items[i] == combo_preview)
				{
					combo_preview = items[i];
					break;
				}
			}
#if 0
			if(ImGui::Combo("##image", &selected_img, items, IM_ARRAYSIZE(items)))
			{
				
				sComp->SetTexture(SelectImage(selected_img));
			}
#elif 1
			if (ImGui::BeginCombo("##combo", combo_preview))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (selected_img == n);
					if (ImGui::Selectable(items[n], is_selected))
					{
						selected_img = n;
						sComp->SetTexture(SelectImage(selected_img));
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
#endif
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

void MyEditor::DrawRigidbody()
{
	RigidbodyComponent* rComp = (RigidbodyComponent*)selectedObj->FindComponent("Rigidbody");
	if (rComp != nullptr)
	{
		//VELOCITY
		float vel_x{ rComp->GetVelocity().x}, vel_y{rComp->GetVelocity().y};
		if (ImGui::TreeNode("Velocity"))
		{
			ImGui::Text("X :"); ImGui::SameLine();
			ImGui::SetNextItemWidth(220.f);
			if (ImGui::SliderFloat("##vel_x", &vel_x, -100.f, 100.f, "%.2f", ImGuiSliderFlags_NoInput))
			{
				vel_x = vel_x;
				rComp->SetVelocity(vel_x, vel_y);
			}
			ImGui::SameLine(); ImGui::SetNextItemWidth(60.f);
			if (ImGui::InputFloat("##input_vel_x", &vel_x, 0.0f, 0.0f, "%.2f"))
			{
				vel_x = std::max(-100.f, std::min(vel_x, 100.f));

				vel_x = vel_x;
				rComp->SetVelocity(vel_x, vel_y);
			}

			ImGui::NewLine();
			ImGui::Text("Y :"); ImGui::SameLine();
			ImGui::SetNextItemWidth(220.f);
			if (ImGui::SliderFloat("##vel_y", &vel_y, -100.f, 100.f, "%.2f", ImGuiSliderFlags_NoInput))
			{
				vel_y = vel_y;
				rComp->SetVelocity(vel_x, vel_y);
			}
			ImGui::SameLine(); ImGui::SetNextItemWidth(60.f);
			if (ImGui::InputFloat("##input_vel_y", &vel_y, 0.0f, 0.0f, "%.2f"))
			{
				vel_y = std::max(-100.f, std::min(vel_y, 100.f));

				vel_y = vel_y;
				rComp->SetVelocity(vel_x, vel_y);
			}

			ImGui::TreePop();
		}

		//DRAG
		float drag{ rComp->GetDrag() };
		if (ImGui::TreeNode("Drag"))
		{
			ImGui::SetNextItemWidth(220.f);
			if (ImGui::SliderFloat("##drag", &drag, 0.f, 360.f, "%.2f", ImGuiSliderFlags_NoInput))
			{
				drag = drag;
				rComp->SetDrag(drag);
			}
			ImGui::SameLine(); ImGui::SetNextItemWidth(60.f);
			if (ImGui::InputFloat("##input_drag", &drag, 0.0f, 0.0f, "%.2f"))
			{
				drag = std::max(0.f, std::min(drag, 10.f));

				drag = drag;
				rComp->SetDrag(drag);
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

void MyEditor::SaveFile()
{
	if (curPath.empty())
		show_make_new_file = true;
	
	else
		GameObjectManager::GetPtr()->SaveAllObjects(curPath.c_str());
}

void MyEditor::CloseFile()
{
	GameObjectManager::GetPtr()->DeleteAllObject();
	selectedObj = nullptr;

	new_obj = false;
	curPath.clear();
	message.clear();
	tmpStr.clear();
	selected = -1;
	selected_obj = 0;
	selected_img = 0;
	selected_type = 0;

	show_new_obj_window = false;
	show_obj_setting = false;
	show_add_comp = false;
	show_make_new_file = false;
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
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImVec2 windowSize = ImVec2(400, 140);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	float centerX = (windowSize.x - 260.f) * 0.5f;

	if(ImGui::Begin("New Object", p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
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

void MyEditor::ShowObjectSetting(bool* p_open)
{
	ImVec2 windowSize = ImVec2(400, 600);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

	if (ImGui::Begin("Object Setting", p_open, ImGuiWindowFlags_NoResize))
	{
		ImGui::SetCursorPosX((windowSize.x - 200.f) * 0.5f);
		if (ImGui::Button("Delete Object", ImVec2(200, 0)))
		{
			message = selectedObj->GetID();
			show_delete_obj = true;
		}
		
		ImGui::NewLine(); ImGui::AlignTextToFramePadding();
		ImGui::Text("ID :");

		std::string buf{};
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		if (ImGui::InputTextWithHint("  ", selectedObj->GetID().c_str(), &buf, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CallbackAlways))
			tmpStr = buf;

		ImGui::SameLine(); ImGui::SetCursorPosX(150.f);
		if (ImGui::Button("Modify", ImVec2(50, 0)))
		{
			if (!filterInput(tmpStr))
			{
				message = "Modification failed. \nOnly letters and numbers are allowed.";
				alarm_window = true;
			}
			else
			{
				selectedObj->SetID(tmpStr);
				GameObjectManager::GetPtr()->ChangeFirstStr(selectedObj, tmpStr);
				if(selectedObj->GetID() == tmpStr)
				{
					message = "Successfully modified.";
					alarm_window = true;
				}
			}
		}
		//Set Object Type Combo
		ImGui::SameLine(); ImGui::SetCursorPosX(230.f);
		ImGui::Text("TYPE :"); ImGui::SameLine();
		const char* items[] = { "PLAYER", "SCENE", "OTHERS" };

		// Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
		const char* combo_preview_value = items[selected_type];
		ImGui::SetNextItemWidth(100.f);
		if (ImGui::BeginCombo("##Type", combo_preview_value, 0))
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				const bool is_selected = (selected_type == n);
				if (ImGui::Selectable(items[n], is_selected))
				{
					selected_type = n;
					//selectedObj->SetType(n);
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
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
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
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

	ImGui::OpenPopup("##Delete component");
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("##Delete component", p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
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
		ImGui::EndPopup();
	}
}

void MyEditor::CreateObjChangeWindow(std::string id)
{
	GameObject* obj = new GameObject(id);
	selectedObj = obj;
	new_obj = true;

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

void MyEditor::ShowAllObjects(bool* p_open)
{
	ImGui::SetNextWindowPos(ImVec2(Helper::W_WIDTH - 202.f, 130.f), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(400, 220), ImGuiCond_Always);
	if (ImGui::Begin("Objects", p_open, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Close", "Ctrl+W")) { *p_open = false; }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// Left
		int i = 0;
		bool y_obj = false;

		ImGuiTextFilter filter;
		filter.Draw("##Object filter", 380.f);
		
		GameObjectManager* gom = GameObjectManager::GetPtr();
		{
			ImGui::BeginChild("left pane", ImVec2(380, 120), ImGuiChildFlags_Border);
			//draw objects
			auto it_begin = gom->Iter_begin;
			for (; it_begin != gom->Iter_end; ++it_begin)
			{
				//First Load
				y_obj = true;
				if(selectedObj == nullptr)
					selectedObj = gom->Iter_begin->second;

				std::string label;
				label = it_begin->first;
				if(filter.PassFilter(label.c_str()))
				{
					if (ImGui::Selectable(label.c_str(), selected_obj == i))
					{
						selected_obj = i;
						selectedObj = gom->FindObjects(label);
					}
					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
						show_obj_setting = true;
				}
				i++;
			}

			if (new_obj) //If created new obj, focus on the new obj
			{
				selected_obj = i - 1;
				new_obj = false;
			}


			ImGui::EndChild();
		}

		// Right
			if (ImGui::Button("Revert")) {}
			ImGui::SameLine();
			if (ImGui::Button("Save")) {}
	}
	ImGui::End();
}

void MyEditor::ShowMakeNewFile(bool* p_open)
{
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImVec2 windowSize = ImVec2(400, 140);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	float centerX = (windowSize.x - 260.f) * 0.5f;

	if (ImGui::Begin("New File", p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
	{
		float textX = ImGui::CalcTextSize("Enter a new file name").x;
		ImGui::SetCursorPosX((windowSize.x - textX) * 0.5f);
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Enter a new file name.");

		std::string buf{};

		ImGui::NewLine();
		ImGui::SetCursorPosX(centerX);
		if (ImGui::InputTextWithHint("  ", "File name", &buf, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CallbackAlways))
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
				{
					curPath = buf;
					curPath = curPath + ".dat";
					//MyFile::CreateDirectory(buf.c_str());
					GameObjectManager::GetPtr()->SaveAllObjects(curPath.c_str());
					show_make_new_file = false;
				}
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
				curPath = tmpStr;
				curPath = curPath + ".dat";
				//MyFile::CreateDirectory(tmpStr.c_str());
				GameObjectManager::GetPtr()->SaveAllObjects(curPath.c_str());
				tmpStr.clear();
				show_make_new_file = false;
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(80, 0)))
			show_make_new_file = false;
	}

	ImGui::End();
}

void MyEditor::AlarmWindow(bool* p_open, std::string msg)
{
	ImVec2 textSize = ImGui::CalcTextSize(msg.c_str());
	ImVec2 windowSize = ImVec2(300, textSize.y + 80.f);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
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
