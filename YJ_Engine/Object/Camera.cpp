#include "Camera.h"

//Camera2D camera2d;

void Camera2D::init(GLFWwindow* pWindow)
{
	// compute camera window's aspect ratio ...
	GLsizei fb_width, fb_height;
	glfwGetFramebufferSize(pWindow, &fb_width, &fb_height);
	ar = static_cast<GLfloat>(fb_width) / fb_height;

	// at startup, the camera must be initialized to free camera ...
	glm::mat3 view = glm::mat3x3(1.f);
	view = glm::mat3(
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		-(position.x), -(position.y), 1.f
	);
	view_xform = view; //world to view mat

	// compute other matrices ...
	glm::mat3 camwin = glm::mat3x3(1.f);
	camwin = glm::mat3(
		2.0 / (float)fb_width, 0.f, 0.f,
		0.f, 2.0 / (float)fb_height, 0.f,
		0.f, 0.f, 1.f
	);
	camwin_to_ndc_xform = camwin;

	world_to_ndc_xform = camwin * view;
}

void Camera2D::update(GLFWwindow* pWindow)
{
	GLsizei fb_width, fb_height;
	GLfloat h = camera2d.height;

	fb_width = ar * h;
	if (camera2d.zoom_flag)
	{
		if (!zoom_max)
		{
			camera2d.height -= camera2d.height_chg_val;
			if (camera2d.height <= camera2d.min_height)
				zoom_max = true;
		}
		else
		{
			camera2d.height += camera2d.height_chg_val;
			if (camera2d.height >= camera2d.max_height)
				zoom_max = false;
		}
		camera2d.zoom_flag = GL_FALSE;
	}

	// compute appropriate world-to-camera view transformation matrix
	float r = glm::radians(degree);
	float s = std::sinf(r);
	float c = std::cosf(r);
	up = glm::vec2(-s, c);
	right = glm::vec2(c, s);

	glm::mat3 view = glm::mat3x3(1.f);
	if (camera2d.camtype_flag)
	{
		float v = -glm::dot(position, up);
		float v2 = -glm::dot(position, right);
		view = glm::mat3(
			c, -s, 0.f,
			s, c, 0.f,
			v2, v, 1.f
		);
	}
	else
	{
		view = glm::mat3(
			1.f, 0.f, 0.f,
			0.f, 1.f, 0.f,
			-(position.x), -(position.y), 1.f
		);
	}

	// at startup, the camera must be initialized to free camera ...
	view_xform = view; //world to view mat

	// compute window-to-NDC transformation matrix
	glm::mat3 camwin = glm::mat3x3(1.f);
	camwin = glm::mat3(
		2.0 / fb_width, 0.f, 0.f,
		0.f, 2.0 / h, 0.f,
		0.f, 0.f, 1.f
	);
	camwin_to_ndc_xform = camwin;

	// compute world-to-NDC transformation matrix
	world_to_ndc_xform = camwin * view;

	//for (auto& obj : GLApp::objects)
	//	obj.second.mdl_to_ndc_xform = world_to_ndc_xform * obj.second.mdl_xform;
}
