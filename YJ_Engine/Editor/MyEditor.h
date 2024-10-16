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
	bool show_new_obj_window = false;
	bool show_obj_setting = false;
	bool show_success_modi_id = false;
	bool alarm_window = false;

	std::string message{};
public:
	std::string tmpStr{};

	static MyEditor* GetPtr();
	static void DeletePtr();

	void Draw();

	void TopBar();
	void ShowMenuFile();
	void ShowCreateNewObjectWindow(bool* p_open);
	void ShowTestObjectSetting(bool* p_open);

	void CreateObjChangeWindow(std::string id);
	void SuccessModifyId(bool* p_open);
	void AlarmWindow(bool* p_open, std::string msg);
};
