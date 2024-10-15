#pragma once

#include <nlohmann/json.hpp>

namespace MyFile
{
	void CreateDirectory(const char* path);
	nlohmann::json LoadData(const char* path);
	void LoadObjectFile(const char* path);
}
