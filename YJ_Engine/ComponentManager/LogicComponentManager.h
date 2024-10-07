#pragma once
#include <string>
#include "GameObject.h"
#include "LogicComponent.h"

//singleton
class LogicComponentManager
{
	LogicComponentManager() = default;
	~LogicComponentManager() = default;
	
	LogicComponentManager(const LogicComponentManager&) = delete;
	const LogicComponentManager& operator=(const LogicComponentManager& other) = delete;

	static LogicComponentManager* logic_ptr;
	list<LogicComponent*> logics;

public:
	static LogicComponentManager* getPtr();
	static void DeletePtr();

	void AddLogic(LogicComponent* lc);
	void DeleteLogic(LogicComponent* c);

	void Update();
};