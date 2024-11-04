#pragma once
#include "../source/imgui/imgui.h"
#include "../source/imgui/imgui_impl_glfw.h"
#include "../source/imgui/imgui_impl_opengl3.h"
#include "../source/imgui/imgui_stdlib.h"

#include "../GSM/GameStateManager.h"
#include "../ComponentManager/GameObjectManager.h"

#include <string>

class MyEditor
{
	MyEditor() = default;
	~MyEditor() = default;

	MyEditor(const MyEditor&) = delete;
	const MyEditor& operator=(const MyEditor& other) = delete;

	static MyEditor* editor_ptr;

private:
	bool alarm_window = false;
	bool show_all_obj = true;
	bool show_new_obj_window = false;
	bool show_obj_setting = false;
	bool show_delete_obj = false;
	bool show_delete_cmp = false;
	bool show_add_comp = false;
	bool show_make_new_file = false;
	bool show_layer_matrix = false;
private:
	bool new_obj = false;

	std::string curPath{};	//For save
	std::string message{};	//For alarm window
	std::string tmpStr{};	//For New Object's
	GameObject* selectedObj = nullptr;

	int selected = -1;		//For list
	int selected_obj = 0;	//For AllObjList
	int selected_img = 0;	//For image combo
	int selected_type = 0;
	int selected_collider_type = 0;
	int selected_layer = 0;

	bool is_window_focus = false;

	//For LayerMask
	bool wallChar = false;
	bool wallEnem = false;
	bool wallBomb = false;
	bool wallItem = false;
	bool wallWall = false;

	bool itemChar = false;
	bool itemEnem = false;
	bool itemBomb = false;
	bool itemItem = false;

	bool bombChar = false;
	bool bombEnem = false;
	bool bombBomb = false;

	bool enemChar = false;
	bool enemEnem = false;

	bool charChar = false;

	//for keyset
	int keySetNumber = 0;
private:
	void AlarmWindow(bool* p_open, std::string msg);
	void ShowCreateNewObjectWindow(bool* p_open);
	void ShowObjectSetting(bool* p_open);
	void ShowDeleteObject(bool* p_open);
	void ShowDeleteComponent(bool* p_open, std::string compName);
	void CreateObjChangeWindow(std::string id);
	void ShowAddComponent(bool* p_open);
	void ShowAllObjects(bool* p_open);
	void ShowMakeNewFile(bool* p_open);
	void ShowLayerMatrix(bool* p_open);

	void TopBar();
	void ComponentListBox();
	void ShowHasComponent();
	void ShowMenuFile();
	void ShowMenuView();
	void DrawComponentProperties(std::string compName);
	void DrawTransform();
	void DrawSprite();
	void DrawPlayer();
	void DrawRigidbody();
	void DrawCollider();
	void DrawLayer();

	const char* SelectImage(int n);
	void SaveFile();
	void CloseFile();
	void UpdateLayerMask();

	GameObject* ClickObject(double cursorX, double cursorY);
	void MoveObjectOnDrag();
public:

	static MyEditor* GetPtr();
	static void DeletePtr();

	void Draw();
};
