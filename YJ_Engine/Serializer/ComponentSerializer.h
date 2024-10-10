#pragma once
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ComponentSerializer
{
public:
	virtual ~ComponentSerializer() = default;
	virtual void LoadFromJson(const json&) = 0;
	virtual json SaveToJson() = 0;
};