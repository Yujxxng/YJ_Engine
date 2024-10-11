#pragma once

#include "../myStd/Helper.h"

#include <iostream>

class Camera2D
{
	Camera2D() = default;
	~Camera2D() = default;

	Camera2D(const Camera2D&) = delete;
	const Camera2D& operator=(const Camera2D& other) = delete;

	static Camera2D* camera_ptr;

public:
	static Camera2D* getPtr();
	static void DeletePtr();

	glm::vec2 position{ 0, 0 };
	float degree = 0.0f;
	glm::vec2 right, up; //Camera control
	glm::mat3 view_xform;
	glm::mat3 camwin_to_ndc_xform;
	glm::mat3 world_to_ndc_xform;

	GLint height{ 600 };
	GLfloat ar;

	GLint min_height{ 500 }, max_height{ 2000 };
	//height is increasing if 1 and decreasing if -1
	GLint height_chg_dir{ 1 };
	// increments by which window height is changed per Z key press
	GLint height_chg_val{ 5 };

	// keyboard button press flags
	GLboolean camtype_flag{ GL_FALSE }; // button V
	GLboolean zoom_flag{ GL_FALSE }; // button Z
	GLboolean zoom_max = false;

	void init(GLFWwindow*);
	void Update(GLFWwindow*);
};