#pragma once
#include "../source/imgui/imgui.h"
#include "../source/imgui/imgui_impl_glfw.h"
#include "../source/imgui/imgui_impl_opengl3.h"
#include "../source/imgui/imgui_stdlib.h"

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
	bool show_new_obj_window = false;
	bool show_obj_setting = false;
	bool show_delete_obj = false;
	bool show_add_comp = false;

	//For alarm window
	std::string message{};
	int selected = -1;
public:
	//For New Object's ID
	std::string tmpStr{};

	static MyEditor* GetPtr();
	static void DeletePtr();

	void Draw();

	void AlarmWindow(bool* p_open, std::string msg);
	void ShowCreateNewObjectWindow(bool* p_open);
	void ShowTestObjectSetting(bool* p_open);
	void ShowDeleteObject(bool* p_open, std::string msg);
	void CreateObjChangeWindow(std::string id);
	void ShowAddComponent(bool* p_open);

	void TopBar();
	void ComponentListBox();
	void ShowMenuFile();
};
