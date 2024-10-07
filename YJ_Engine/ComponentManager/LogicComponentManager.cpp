#include "LogicComponentManager.h"

LogicComponentManager* LogicComponentManager::logic_ptr = nullptr;


LogicComponentManager* LogicComponentManager::getPtr()
{
    if (logic_ptr == nullptr)
    {
        logic_ptr = new LogicComponentManager;

        return logic_ptr;
    }
    else
        return logic_ptr;
}

void LogicComponentManager::DeletePtr()
{
    if (logic_ptr != nullptr)
    {
        delete logic_ptr;
        logic_ptr = nullptr;
    }
}


void LogicComponentManager::AddLogic(LogicComponent* lc)
{
    logics.push_back(lc);
}

void LogicComponentManager::DeleteLogic(LogicComponent* c)
{
    logics.remove(c);
}


void LogicComponentManager::Update()
{
    for (auto it = logics.begin(); it != logics.end(); it++)
    {
        (*it)->Update();
    }
}
