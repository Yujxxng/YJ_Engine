#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

class Shader
{
	char vertex[1024] = "";
	char fragment[1024] = "";

public:
	unsigned int ID{};

	//Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	void unUse();
	
	void setShader(const char* vertexPath, const char* fragmentPath);
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

	const char* GetVertexName();
	const char* GetFragmentName();

private:
	void checkCompileErrors(unsigned int shader, std::string type);
};