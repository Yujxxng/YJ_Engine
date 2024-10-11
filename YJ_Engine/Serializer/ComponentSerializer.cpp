#include "ComponentSerializer.h"
#include <iostream>
#include <fstream>

json ComponentSerializer::LoadData(const char* path)
{
	std::ifstream jf(path);
	if (!jf.is_open())
	{
		std::cout << "FILE NOT FOUND " << path << std::endl;
		std::ofstream ofs(path);
		ofs.close();
		return;
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
