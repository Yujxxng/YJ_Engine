#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <FreeImage.h>

void Window_Resized(GLFWwindow* window, int width, int height);

void Key_Pressed(GLFWwindow* window, int key, int scancode, int action, int mods);
void show_glfw_error(int error, const char* description);
void PrintVersionInfo();

const char* vertexShaderSource =
								"#version 330 core\n"
								"layout (location = 0) in vec3 aPos;\n"
								"void main()\n"
								"{\n"
								"   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
								"}\0";
const char* vertexShaderSource2 =
								"#version 330 core\n"
								"layout (location = 0) in vec3 aPos;\n"
								"void main()\n"
								"{\n"
								"   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
								"}\0";

const char* fragmentShaderSouce =
								"#version 330 core\n"
								"out vec4 FragColor;\n"
								"void main()\n"
								"{\n"
								"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
								"}\0";
const char* fragmentShaderSouce2 =
								"#version 330 core\n"
								"out vec4 FragColor;\n"
								"void main()\n"
								"{\n"
								"	FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
								"}\0";

/*
bool InitShaderProgram()
{
	const GLchar* ShaderSource =
		"#version 330 core\n"
		"in vec3 position;\n"
		"in vec3 color;\n"
		"out vec3 ColorAttr;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(position, 1.0);\n"
		"ColorAttr = color;\n"
		"}\0";

	const GLchar* fragmentShaderSource =
		"#version 330 core\n"
		"in vec3 fcolor;"
		"out vec4 fragmentColor;"
		"void main()"
		"{"
		"fragmentColor = vec4(fcolor, 1.0);"
		"}";

	GLuint vertextShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertextShader, 1, &ShaderSource, NULL);
	glCompileShader(vertextShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertextShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertextShader, 512, NULL, errorLog);
		std::cerr << "ERROR : Fail to compile the vertex shader\n" << errorLog << std::endl;
		glDeleteShader(vertextShader);
		return false;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR : Fail to compile the fragment shader\n" << errorLog << std::endl;
		return false;
	}

	ShaderID = glCreateProgram();

	glAttachShader(ShaderID, vertextShader);
	glAttachShader(ShaderID, fragmentShader);
	glLinkProgram(ShaderID);

	glDeleteShader(vertextShader);
	glDeleteShader(fragmentShader);

	glGetProgramiv(ShaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(ShaderID, 512, NULL, errorLog);
		std::cerr << "ERROR : Fail to link to shader program\n" << errorLog << std::endl;
		return false;
	}

	return true;
}

bool DefineVAO()
{
	float position[] =
	{
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	float color[] =
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

	glGenBuffers(1, &color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLint positionAttr = glGetAttribLocation(ShaderID, "position");
	if (positionAttr == -1)
	{
		std::cerr << "fail to set a position" << std::endl;
		return false;
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(positionAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttr);

	glUseProgram(ShaderID);
	GLint colorAttr = glGetAttribLocation(ShaderID, "color");
	if (colorAttr == -1)
	{
		std::cerr << "fail to set a color" << std::endl;
		return false;
	}
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glVertexAttribPointer(colorAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colorAttr);

	glBindVertexArray(0);

	return true;
}
*/
int main()
{
	glfwSetErrorCallback(show_glfw_error);


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


	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Example", NULL, NULL);
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
	/*
	if (!InitShaderProgram())
	{
		std::cerr << "ERROR : fail to ganerate shader program" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	if (!DefineVAO())
	{
		std::cerr << "ERROR : fail to generate VAO" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	glUseProgram(ShaderID);
	glBindVertexArray(vao);
	*/

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int vertexShader2 = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader2, 1, &vertexShaderSource2, NULL);

	glCompileShader(vertexShader);
	glCompileShader(vertexShader2);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSouce, NULL);
	glCompileShader(fragmentShader);

	unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSouce2, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	unsigned int shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader2);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader2);
	glDeleteShader(fragmentShader2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	/*
	glm::vec2 vertices[] =
	{
		glm::vec2(0.5f, 0.5f),
		glm::vec2(0.5f, -0.5f),
		glm::vec2(-0.5f, -0.5f),
		glm::vec2(-0.5f, 0.5f),
	};
	*/
	glm::vec2 vertices[] =
	{
		glm::vec2(-0.9f, -0.5f),
		glm::vec2(-0.0f, -0.5f),
		glm::vec2(-0.45f, -0.5f),
	};

	glm::vec2 vertices2[] =
	{
		glm::vec2(-0.0f, -0.5f),
		glm::vec2(0.9f, -0.5f),
		glm::vec2(0.45f, 0.5f)
	};

	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	unsigned int VBO, VAO, EBO, VBO2, VAO2;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices2, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

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

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUseProgram(shaderProgram2);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteProgram(shaderProgram2);

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