#pragma once
#include "GraphicComponent.h"
#include "list"

//singleton
class GraphicComponentManager
{
	GraphicComponentManager() = default;
	~GraphicComponentManager() = default;
	
	GraphicComponentManager(const GraphicComponentManager&) = delete;
	const GraphicComponentManager& operator=(const GraphicComponentManager& other) = delete;

	static GraphicComponentManager* graphic_ptr;
	std::list<GraphicComponent*> graphics;

public:
	static GraphicComponentManager* GetPtr();
	static void DeletePtr();

	void AddGraphic(GraphicComponent* lc);
	void DeleteGraphic(GraphicComponent* c);

	void Update();
};