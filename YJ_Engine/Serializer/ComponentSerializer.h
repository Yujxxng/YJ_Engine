#pragma once
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#define GameDataName "app.dat"

class ComponentSerializer
{
public:
	virtual ~ComponentSerializer() = default;
	virtual void LoadFromJson(const json& data) = 0;
	virtual json SaveToJson() = 0;

	void CreateDirectory(const char* path);
	json LoadData(const char* path);
	//void SaveData(const char* path);
};