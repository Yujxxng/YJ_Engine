#include "ComponentSerializer.h"
#include <iostream>
#include <fstream>

void ComponentSerializer::CreateDirectory(const char* path)
{
	std::cout << "FILE NOT FOUND " << path << std::endl;
	std::ofstream ofs(path);
	ofs.close();
}

json ComponentSerializer::LoadData(const char* path)
{
	std::ifstream jf(path);
	if (!jf.is_open())
	{
		CreateDirectory(path);
		return nullptr;
	}

	using json = nlohmann::json;
	json data;
	jf.seekg(0, std::ios::end);
	if (jf.tellg() != 0) {
		jf.seekg(0);
		jf >> data;
	}
	jf.close();
	
	return data;
}
/*
void ComponentSerializer::SaveData(const char* path)
{
	std::ifstream ifs(path);
	if (!ifs.is_open())
	{
		std::cout << "FILE NOT FOUND" << std::endl;
		std::ofstream ofs(path);
		ofs.close();
		return;
	}

	using json = nlohmann::json;
	json data;
	ifs.seekg(0, std::ios::end);
	if (ifs.tellg() != 0) {
		ifs.seekg(0);
		ifs >> data;
	}
	ifs.close();

	//json compData;
}
*/