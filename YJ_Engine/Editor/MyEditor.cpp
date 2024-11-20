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
#include "../Components/ColliderComponent.h"

#include "../Object/BombManager.h"

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
	if(is_window_focus && ImGui::IsMouseReleased(0))
		is_window_focus = false;

	if (ImGui::IsMouseClicked(1))
	{
		double cursorX, cursorY;
		glfwGetCursorPos(Helper::ptr_window, &cursorX, &cursorY);
		cursorX = cursorX - (Helper::W_WIDTH / 2.f);
		cursorY = (Helper::W_HEIGHT / 2.f) - cursorY;

		selectedObj = ClickObject(cursorX, cursorY);

		if (selectedObj == nullptr)
			is_window_focus = false;
	}

	if (!is_window_focus)
	{
		MoveObjectOnDrag();
	}

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

	if (show_layer_matrix)
		ShowLayerMatrix(&show_layer_matrix);

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

		if (ImGui::BeginMenu("Edit"))
		{
			ShowMenuEdit();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ShowMenuView();
			ImGui::EndMenu();
		}


		//Edit Mode Button
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
	const char* components[] = { "Transform", "Sprite", "Player", "Rigidbody", "Collider", "Timer", "Spawn"};

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
		SaveFile();
	}
	if (ImGui::MenuItem("Save As.."))
	{
		std::cout << "Save As.." << std::endl;
	}
	if (ImGui::MenuItem("Close", "Alt+F4"))
	{
		GSM::GameStateManager::GetGSMPtr()->gGameRunning = 0;
	}
}

void MyEditor::ShowMenuView()
{
	if (ImGui::MenuItem("Layer"))
	{
		show_layer_matrix = true;
	}
}

void MyEditor::ShowMenuEdit()
{
	if (ImGui::MenuItem("Copy Object", "Ctrl+J"))
	{
		GameObject* obj = new GameObject("copy");
		selectedObj->CopyThisObject(obj);
		selectedObj = obj;
	}
	if (ImGui::MenuItem("Delete", "Delete"))
	{
		0;
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
	else if (compName == "Collider")
		DrawCollider();
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

		//SHADER
		if (ImGui::TreeNode("Shader"))
		{
			const char* shaders[] = { "Assets/Shaders/shader.fs", "Assets/Shaders/grid.frag" };
			const char* shader_preview = sComp->GetFragmentShaderName();
			for (int i = 0; i < IM_ARRAYSIZE(shaders); i++)
			{
				if (shaders[i] == shader_preview)
				{
					shader_preview = shaders[i];
					break;
				}
			}

			if (ImGui::BeginCombo("##combo", shader_preview))
			{
				for (int n = 0; n < IM_ARRAYSIZE(shaders); n++)
				{
					const bool is_selected = (selected_sdr == n);
					if (ImGui::Selectable(shaders[n], is_selected))
					{
						selected_sdr = n;
						sComp->SetShader(sComp->GetVertexShaderName(), shaders[selected_sdr]);
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			ImGui::TreePop();
		}

		//IMAGE
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
			ImGui::TreePop();
		}
	}
}

void MyEditor::DrawPlayer()
{
	PlayerComponent* pComp = (PlayerComponent*)selectedObj->FindComponent("Player");
	if (pComp != nullptr)
	{
		//KEYSET
		if(ImGui::RadioButton("None", &keySetNumber, 0))
			pComp->ClearKeySet(); 
		
		ImGui::SameLine();
		if (ImGui::RadioButton("1P", &keySetNumber, 1))
		{
			pComp->ClearKeySet();
			pComp->AddKey("A");
			pComp->AddKey("S");
			pComp->AddKey("D");
			pComp->AddKey("W");
			pComp->AddKey("LEFT_SHIFT");
		}

		ImGui::SameLine();
		if (ImGui::RadioButton("2P", &keySetNumber, 2))
		{
			pComp->ClearKeySet();
			pComp->AddKey("LEFT");
			pComp->AddKey("RIGHT");
			pComp->AddKey("UP");
			pComp->AddKey("DOWN");
			pComp->AddKey("RIGHT_SHIFT");
		}

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

void MyEditor::DrawCollider()
{
	ColliderComponent *cComp = (ColliderComponent*)selectedObj->FindComponent("Collider");
	if (cComp != nullptr)
	{
		ImGui::Text("Visible"); ImGui::SameLine();
		ImGui::Checkbox("##collider_visible", &cComp->show);

		ImGui::Text("TYPE :"); ImGui::SameLine();
		const char* collider_types[] = { "AABB", "CIRCLE" };
		const char* collider_type_combo_value = collider_types[selected_collider_type];

		ImGui::SetNextItemWidth(100.f);
		if (ImGui::BeginCombo("##ColliderType", collider_type_combo_value, 0))
		{
			for (int n = 0; n < IM_ARRAYSIZE(collider_types); n++)
			{
				const bool is_selected = (selected_collider_type == n);
				if (ImGui::Selectable(collider_types[n], is_selected))
				{
					selected_collider_type = n;
					//selectedObj->SetType(n);
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();
		ImGui::Text("Layer :"); ImGui::SameLine();
		const char* layer_types[] = { "None", "Character", "Enemy", "Wall", "Bomb", "Item" };
		const char* layer_combo_value = cComp->GetLayerToString();//= layer_types[selected_layer];

		ImGui::SetNextItemWidth(100.f);
		if (ImGui::BeginCombo("##ColliderLayerType", layer_combo_value, 0))
		{
			for (int n = 0; n < IM_ARRAYSIZE(layer_types); n++)
			{
				const bool is_selected = (selected_layer == n);
				if (ImGui::Selectable(layer_types[n], is_selected))
				{
					selected_layer = n;

					if (selected_layer == 0)
						cComp->SetLayer(LAYER::NONE);
					else if (selected_layer == 1)
						cComp->SetLayer(LAYER::CHARACTER);
					else if (selected_layer == 2)
						cComp->SetLayer(LAYER::ENEMY);
					else if (selected_layer == 3)
						cComp->SetLayer(LAYER::WALL);
					else if (selected_layer == 4)
						cComp->SetLayer(LAYER::BOMB);
					else if (selected_layer == 5)
						cComp->SetLayer(LAYER::ITEM);
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (selected_collider_type == 1)
		{
			cComp->SetType(COLLIDER_TYPE::CIRCLE);
			float radius = cComp->GetRadius();
			ImGui::Text("Radius:"); ImGui::SameLine();
			ImGui::SetNextItemWidth(220.f);
			if (ImGui::SliderFloat("##collider_radius", &radius, 0.f, 100.f, "%.2f", ImGuiSliderFlags_NoInput))
			{
				radius = radius;
				cComp->SetRadius(radius);
			}
			ImGui::SameLine(); ImGui::SetNextItemWidth(60.f);
			if (ImGui::InputFloat("##collider_input_radius", &radius, 0.0f, 0.0f, "%.2f"))
			{
				radius = radius;
				cComp->SetRadius(radius);
			}
		}

		//POSITION
		float pos_x{ cComp->GetPos().x}, pos_y{cComp->GetPos().y};
		if (ImGui::TreeNode("Position"))
		{
			ImGui::Text("X :"); ImGui::SameLine();
			ImGui::SetNextItemWidth(220.f);
			if (ImGui::SliderFloat("##collider_pos_x", &pos_x, -1000.f, 1000.f, "%.2f", ImGuiSliderFlags_NoInput))
			{
				pos_x = pos_x;
				cComp->SetPos(pos_x, pos_y);
			}
			ImGui::SameLine(); ImGui::SetNextItemWidth(60.f);
			if (ImGui::InputFloat("##collider_input_pos_x", &pos_x, 0.0f, 0.0f, "%.2f"))
			{
				pos_x = std::max(-100.f, std::min(pos_x, 1000.f));

				pos_x = pos_x;
				cComp->SetPos(pos_x, pos_y);
			}

			ImGui::NewLine();
			ImGui::Text("Y :"); ImGui::SameLine();
			ImGui::SetNextItemWidth(220.f);
			if (ImGui::SliderFloat("##collider_pos_y", &pos_y, -1000.f, 1000.f, "%.2f", ImGuiSliderFlags_NoInput))
			{
				pos_y = pos_y;
				cComp->SetPos(pos_x, pos_y);
			}
			ImGui::SameLine(); ImGui::SetNextItemWidth(60.f);
			if (ImGui::InputFloat("##collider_input_pos_y", &pos_y, 0.0f, 0.0f, "%.2f"))
			{
				pos_y = std::max(-100.f, std::min(pos_y, 1000.f));

				pos_y = pos_y;
				cComp->SetPos(pos_x, pos_y);
			}

			ImGui::TreePop();
		}

		//SIZE
		float size_x{ cComp->GetSize().x }, size_y{ cComp->GetSize().y };
		if (ImGui::TreeNode("Size"))
		{
			ImGui::Text("X :"); ImGui::SameLine();
			ImGui::SetNextItemWidth(220.f);
			if (ImGui::SliderFloat("##collider_size_x", &size_x, -1000.f, 1000.f, "%.2f", ImGuiSliderFlags_NoInput))
			{
				size_x = size_x;
				cComp->SetSize(size_x, size_y);
			}
			ImGui::SameLine(); ImGui::SetNextItemWidth(60.f);
			if (ImGui::InputFloat("##collider_input_size_x", &size_x, 0.0f, 0.0f, "%.2f"))
			{
				size_x = std::max(-100.f, std::min(size_x, 1000.f));

				size_x = size_x;
				cComp->SetSize(size_x, size_y);
			}

			ImGui::NewLine();
			ImGui::Text("Y :"); ImGui::SameLine();
			ImGui::SetNextItemWidth(220.f);
			if (ImGui::SliderFloat("##collider_size_y", &size_y, -1000.f, 1000.f, "%.2f", ImGuiSliderFlags_NoInput))
			{
				size_y = size_y;
				cComp->SetSize(size_x, size_y);
			}
			ImGui::SameLine(); ImGui::SetNextItemWidth(60.f);
			if (ImGui::InputFloat("##collider_input_size_y", &size_y, 0.0f, 0.0f, "%.2f"))
			{
				size_y = std::max(-100.f, std::min(size_y, 1000.f));

				size_y = size_y;
				cComp->SetSize(size_x, size_y);
			}
			ImGui::TreePop();
		}
	}
}

void MyEditor::DrawLayer()
{
	if (ImGui::BeginTable("##Layer table", 6))
	{
		ImGui::TableNextColumn(); ImGui::Text("         ");
		ImGui::TableNextColumn(); ImGui::Text("Character"); 
		ImGui::TableNextColumn(); ImGui::Text("Enemy");
		ImGui::TableNextColumn(); ImGui::Text("Bomb"); 
		ImGui::TableNextColumn(); ImGui::Text("Item"); 
		ImGui::TableNextColumn(); ImGui::Text("Wall"); 

		ImGui::TableNextColumn(); ImGui::Text("Wall"); 
		ImGui::TableNextColumn(); ImGui::Checkbox("##Wall&Char", &wallChar);
		ImGui::TableNextColumn(); ImGui::Checkbox("##Wall&Enem", &wallEnem);
		ImGui::TableNextColumn(); ImGui::Checkbox("##Wall&Bomb", &wallBomb);
		ImGui::TableNextColumn(); ImGui::Checkbox("##Wall&Item", &wallItem);
		ImGui::TableNextColumn(); ImGui::Checkbox("##Wall&Wall", &wallWall);

		ImGui::TableNextColumn(); ImGui::Text("Item");
		ImGui::TableNextColumn(); ImGui::Checkbox("##Item&Char", &itemChar);
		ImGui::TableNextColumn(); ImGui::Checkbox("##Item&Enem", &itemEnem);
		ImGui::TableNextColumn(); ImGui::Checkbox("##Item&Bomb", &itemBomb);
		ImGui::TableNextColumn(); ImGui::Checkbox("##Item&Item", &itemItem);
		ImGui::TableNextRow();

		ImGui::TableNextColumn(); ImGui::Text("Bomb");
		ImGui::TableNextColumn(); ImGui::Checkbox("##Bomb&Char", &bombChar);
		ImGui::TableNextColumn(); ImGui::Checkbox("##Bomb&Enem", &bombEnem);
		ImGui::TableNextColumn(); ImGui::Checkbox("##Bomb&Bomb", &bombBomb);
		ImGui::TableNextRow();

		ImGui::TableNextColumn(); ImGui::Text("Enemy");
		ImGui::TableNextColumn(); ImGui::Checkbox("##Enem&Char", &enemChar);
		ImGui::TableNextColumn(); ImGui::Checkbox("##Enem&Enem", &enemEnem);
		ImGui::TableNextRow();

		ImGui::TableNextColumn(); ImGui::Text("Character");
		ImGui::TableNextColumn(); ImGui::Checkbox("##Char&Char", &charChar);

		ImGui::EndTable();
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

void MyEditor::UpdateLayerMask()
{
	if (wallChar)	CollisionManager::GetPtr()->SetLayerMask(WALL, CHARACTER);
	else			CollisionManager::GetPtr()->RemoveLayerMask(WALL, CHARACTER);
	if (wallEnem)	CollisionManager::GetPtr()->SetLayerMask(WALL, ENEMY);
	else			CollisionManager::GetPtr()->RemoveLayerMask(WALL, ENEMY);
	if (wallBomb)	CollisionManager::GetPtr()->SetLayerMask(WALL, BOMB);
	else			CollisionManager::GetPtr()->RemoveLayerMask(WALL, BOMB);
	if (wallItem)	CollisionManager::GetPtr()->SetLayerMask(WALL, ITEM);
	else			CollisionManager::GetPtr()->RemoveLayerMask(WALL, ITEM);
	if (wallWall)	CollisionManager::GetPtr()->SetLayerMask(WALL, WALL);
	else			CollisionManager::GetPtr()->RemoveLayerMask(WALL, WALL);

	if (itemChar)	CollisionManager::GetPtr()->SetLayerMask(ITEM, CHARACTER);
	else			CollisionManager::GetPtr()->RemoveLayerMask(ITEM, CHARACTER);
	if (itemEnem)	CollisionManager::GetPtr()->SetLayerMask(ITEM, ENEMY);
	else			CollisionManager::GetPtr()->RemoveLayerMask(ITEM, ENEMY);
	if (itemBomb)	CollisionManager::GetPtr()->SetLayerMask(ITEM, BOMB);
	else			CollisionManager::GetPtr()->RemoveLayerMask(ITEM, BOMB);
	if (itemItem)	CollisionManager::GetPtr()->SetLayerMask(ITEM, ITEM);
	else			CollisionManager::GetPtr()->RemoveLayerMask(ITEM, ITEM);

	if (bombChar)	CollisionManager::GetPtr()->SetLayerMask(BOMB, CHARACTER);
	else			CollisionManager::GetPtr()->RemoveLayerMask(BOMB, CHARACTER);
	if (bombEnem)	CollisionManager::GetPtr()->SetLayerMask(BOMB, ENEMY);
	else			CollisionManager::GetPtr()->RemoveLayerMask(BOMB, ENEMY);
	if (bombBomb)	CollisionManager::GetPtr()->SetLayerMask(BOMB, BOMB);
	else			CollisionManager::GetPtr()->RemoveLayerMask(BOMB, BOMB);

	if (enemChar)	CollisionManager::GetPtr()->SetLayerMask(ENEMY, CHARACTER);
	else			CollisionManager::GetPtr()->RemoveLayerMask(ENEMY, CHARACTER);
	if (enemEnem)	CollisionManager::GetPtr()->SetLayerMask(ENEMY, ENEMY);
	else			CollisionManager::GetPtr()->RemoveLayerMask(ENEMY, ENEMY);

	if (charChar)	CollisionManager::GetPtr()->SetLayerMask(CHARACTER, CHARACTER);
	else			CollisionManager::GetPtr()->RemoveLayerMask(CHARACTER, CHARACTER);
}

GameObject* MyEditor::ClickObject(double cursorX, double cursorY)
{
	GameObject* tmp = GameObjectManager::GetPtr()->FindObjects(cursorX, cursorY);

	return tmp;
}

void MyEditor::MoveObjectOnDrag()
{
	if (ImGui::IsMouseDragging(0))
	{
		ImVec2 dragDelta = ImGui::GetMouseDragDelta(0);

		TransformComponent* tComp = (TransformComponent*)selectedObj->FindComponent("Transform");
		if(tComp)
		{
			glm::vec2 objPosition = tComp->GetPos();
			tComp->SetPos(objPosition.x + dragDelta.x, objPosition.y - dragDelta.y);

			ImGui::ResetMouseDragDelta(0);
		}
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
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImVec2 windowSize = ImVec2(400, 140);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	float centerX = (windowSize.x - 260.f) * 0.5f;

	if(ImGui::Begin("New Object", p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
	{
		if (ImGui::IsWindowFocused())
			is_window_focus = true;

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
		if (ImGui::IsWindowFocused())
			is_window_focus = true;

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
		if (ImGui::IsWindowFocused())
			is_window_focus = true;

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
		if (ImGui::IsWindowFocused())
			is_window_focus = true;

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
		if (ImGui::IsWindowFocused())
			is_window_focus = true;

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
		if (ImGui::IsWindowFocused())
			is_window_focus = true;

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

void MyEditor::ShowLayerMatrix(bool* p_open)
{
	ImVec2 windowSize = ImVec2(400, 170);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	if (ImGui::Begin("Layer Matrix", p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
	{
		if (ImGui::IsWindowFocused())
			is_window_focus = true;

		DrawLayer();
		UpdateLayerMask();
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
		if (ImGui::IsWindowFocused())
			is_window_focus = true;

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
