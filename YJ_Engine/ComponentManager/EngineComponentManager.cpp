#include "EngineComponentManager.h"

EngineComponentManager* EngineComponentManager::engine_ptr = nullptr;

EngineComponentManager* EngineComponentManager::getPtr()
{
    if (engine_ptr == nullptr)
    {
        engine_ptr = new EngineComponentManager;

        return engine_ptr;
    }
    else
        return engine_ptr;
}

void EngineComponentManager::DeletePtr()
{
    if (engine_ptr != nullptr)
    {
        delete engine_ptr;
        engine_ptr = nullptr;
    }
}


void EngineComponentManager::AddEngine(EngineComponent* lc)
{
    engines.push_back(lc);
}

void EngineComponentManager::DeleteEngine(EngineComponent* c)
{
    engines.remove(c);
}

void EngineComponentManager::Update()
{
    for (auto it = engines.begin(); it != engines.end(); it++)
    {
        (*it)->Update();
    }
}
