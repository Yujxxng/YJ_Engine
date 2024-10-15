#pragma once
#include "../source/imgui/imgui.h"
#include "../source/imgui/imgui_impl_glfw.h"
#include "../source/imgui/imgui_impl_opengl3.h"
#include "../source/imgui/imgui_stdlib.h"


class MyEditor
{
	MyEditor() = default;
	~MyEditor() = default;

	MyEditor(const MyEditor&) = delete;
	const MyEditor& operator=(const MyEditor& other) = delete;

	static MyEditor* editor_ptr;

private:
	bool show_new_obj_window = false;

public:
	static MyEditor* GetPtr();
	static void DeletePtr();

	void Draw();

	void TopBar();
	void ShowMenuFile();
	void ShowCreateNewObjectWindow(bool* p_open);
};