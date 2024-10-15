#pragma once
#include "GameObject.h"
#include "EngineComponent.h"
#include "list"

class EngineComponentManager
{
	EngineComponentManager() = default;
	~EngineComponentManager() = default;
	
	EngineComponentManager(const EngineComponentManager&) = delete;
	const EngineComponentManager& operator=(const EngineComponentManager& other) = delete;

	static EngineComponentManager* engine_ptr;
	std::list<EngineComponent*> engines;

public:
	static EngineComponentManager* GetPtr();
	static void DeletePtr();

	void AddEngine(EngineComponent* lc);
	void DeleteEngine(EngineComponent* c);
	void Update();
};