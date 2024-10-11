#pragma once
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#define GameDataName "app.dat"

class ComponentSerializer
{
public:
	virtual ~ComponentSerializer() = default;
	virtual void LoadFromJson() = 0;
	virtual void SaveToJson() = 0;

	json LoadData(const char* path);
	void SaveData(const char* path);
};