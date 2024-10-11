#pragma once

#ifndef HELPER_H
#define HELPER_H

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <string>

struct Helper
{
	static bool init(GLint width, GLint height, std::string title);
	static void cleanUp();

	//callbacks
	static void error_cb(int error, char const* description);
	static void fbsize_cb(GLFWwindow* ptr_win, int width, int height);

	static void key_cb(GLFWwindow* pwin, int key, int scancode, int action, int mod);
	static void mousebutton_cb(GLFWwindow* pwin, int button, int action, int mod);
	static void mousescroll_cb(GLFWwindow* pwin, double xoffset, double yoffset);
	static void mousepos_cb(GLFWwindow* pwin, double xpos, double ypos);

	static void setup_event_callbacks();

	static void print_specs();

	static GLint W_WIDTH, W_HEIGHT;
	static GLdouble fps;
	static GLdouble delta_time; // time taken to complete most recent game loop
	static std::string W_TITLE;
	static GLFWwindow* ptr_window;
	//static GLfloat clear_color[4]; // rgba normalize components in range 0.0f to 1.0f
};

#endif HELPER_H