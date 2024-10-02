#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <FreeImage.h>
#include <filesystem>

#include "Shader.h"

void Window_Resized(GLFWwindow* window, int width, int height);

void Key_Pressed(GLFWwindow* window, int key, int scancode, int action, int mods);
void show_glfw_error(int error, const char* description);
void PrintVersionInfo();

void copyAssets()
{
	std::filesystem::path sourceDir = std::filesystem::current_path().parent_path().parent_path() / "Assets\\Shaders";
	std::filesystem::path targetDir = std::filesystem::current_path() / "Assets\\Shaders";

	std::filesystem::create_directories(targetDir);
	for (const auto& entry : std::filesystem::directory_iterator(sourceDir))
		std::filesystem::copy(entry.path(), targetDir / entry.path().filename(), std::filesystem::copy_options::overwrite_existing);	
}

GLuint CreateTexture(char const* fileName)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName, 0);

	if (format == -1)
	{
		std::cerr << "NOT FOUND THE IMAGE : " << fileName << std::endl;
	}
	if (format == FIF_UNKNOWN)
	{
		std::cerr << "NOT FOUNT FILE FORMAT - ATTEMPT TO GET FROM FILE EXTENSION.." << std::endl;
		
		format = FreeImage_GetFIFFromFilename(fileName);
		if (!FreeImage_FIFSupportsReading(format))
	{
		std::cerr << "IMAGE FORMAT CANNOT BE READ" << std::endl;

	}
	}

	FIBITMAP* bitmap = FreeImage_Load(format, fileName);
	int bitsPerPixel = FreeImage_GetBPP(bitmap);

	FIBITMAP* bitmap32;
	if (bitsPerPixel == 32)
		bitmap32 = bitmap;
	else
		bitmap32 = FreeImage_ConvertTo32Bits(bitmap);

	int imageWidth = FreeImage_GetWidth(bitmap32);
	int imageHeight = FreeImage_GetHeight(bitmap32);

	GLubyte* texData = FreeImage_GetBits(bitmap32);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, texData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glBindTexture(GL_TEXTURE_2D, 0);

	FreeImage_Unload(bitmap32);

	if (bitsPerPixel != 32)
		FreeImage_Unload(bitmap);

	return texture;
}

int main()
{
	glfwSetErrorCallback(show_glfw_error);
	copyAssets();
	// Initialize the library
	if (!glfwInit()) {
		std::cerr << "fail to init GLFW" << '\n';
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);	

	GLFWwindow* window = glfwCreateWindow(800, 600, "YJ_Engine", NULL, NULL);
	if (!window)
	{
		std::cerr << "fail to genetate window" << '\n';
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, Window_Resized);
	glfwSetKeyCallback(window, Key_Pressed);

	//Set Vsync (Buffer swapping, 60fps)
	glfwSwapInterval(1);

	//Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum errorCode = glewInit();
	if (errorCode != GLEW_OK) {
		std::cerr << "fali to init GLEW " << glewGetErrorString(errorCode) << std::endl;
		glfwTerminate();
		exit(-1);
	}
	PrintVersionInfo();

	int nr_extensions = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &nr_extensions);

	glClearColor(0, 0, 0, 1);

	Shader myShader("Assets/Shaders/shader.vs", "Assets/Shaders/shader.fs");

	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	float texCoords[] =
	{
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	GLuint tex = CreateTexture("Assets/manggom.png");

	
	double lastTime = glfwGetTime();
	int frames = 0;
	int count = 0;

	while (!glfwWindowShouldClose(window)) 
	{
		count++;

		double currentTime = glfwGetTime();
		frames++;

		if (currentTime - lastTime >= 1.0f)
		{
			std::cout << 1000.0f / frames << "ms/frame | " << frames << "fps" << std::endl;
			frames = 0;
			lastTime = currentTime;
		}
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		myShader.use();
		
		glBindTexture(GL_TEXTURE_2D, tex);
		glBindVertexArray(VAO);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void show_glfw_error(int error, const char* description) {
	std::cerr << "Error: " << description << '\n';
}

void PrintVersionInfo()
{
	std::cout << "OpenGL version : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Vendor : " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer : " << glGetString(GL_RENDERER) << std::endl;
}

void Window_Resized(GLFWwindow* window, int width, int height) {
	std::cout << "Window resized, new window size: " << width << " x " << height << '\n';

	glClearColor(0, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);
}

void Key_Pressed(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == 'Q' && action == GLFW_PRESS) {
		glfwTerminate();
		exit(0);
	}
}