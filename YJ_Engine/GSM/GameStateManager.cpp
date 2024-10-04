#include "GameStateManager.h"
#include "BaseLevel.h"
//#include "..\ComponentManager\LogicComponentManager.h"
//#include "..\ComponentManager\LogicComponent.h"
//#include "..\ComponentManager\EngineComponentManager.h"
//#include "..\ComponentManager\GraphicComponentManager.h"
//#include "..\ComponentManager\EventManager.h"
//#include "..\ComponentManager\ResourceManager.h"
#include <iostream>

GSM::GameStateManager* GSM::GameStateManager::ptr = nullptr;

GSM::GameStateManager::GameStateManager() : currentLevel(nullptr), nextLevel(nullptr)
{
}

GSM::GameStateManager::~GameStateManager()
{
    if (currentLevel)
    {
        if (currentLevel == nextLevel)
            nextLevel = nullptr;
        delete currentLevel;
    }

    if (nextLevel)
        delete nextLevel;
}

GSM::GameStateManager* GSM::GameStateManager::GetGSMPtr()
{
    if (ptr == nullptr)
    {
        ptr = new GameStateManager;
    }

    return ptr;
}

void GSM::GameStateManager::DeleteGSM()
{
    if (ptr)
    {
        delete ptr;
        ptr = nullptr;
    }
}

void GSM::GameStateManager::Init()
{
    if (nextLevel)
    {
        nextLevel->Init();
    }
}

void GSM::GameStateManager::Update()
{
    if (currentLevel != nextLevel)
    {
        Exit(); //currentLevel
        Init(); //nextLevel

        currentLevel = nextLevel;
    }

    if (currentLevel)
    {
        //LogicComponentManager::getPtr()->Update();
        //EngineComponentManager::getPtr()->Update();
        //GraphicComponentManager::getPtr()->Update();

        currentLevel->Update();
    }
}

void GSM::GameStateManager::Exit()
{
    if (currentLevel)
    {
        if(currentLevel == nextLevel)
            nextLevel = nullptr;

        currentLevel->Exit();

        delete currentLevel;
        currentLevel = nullptr;
    }
   //LogicComponentManager::DeletePtr();
   //EngineComponentManager::DeletePtr();
   //GraphicComponentManager::DeletePtr();
    //EventManager::DeletePtr();
}

void GSM::GameStateManager::ChangeLevel(BaseLevel* newLvl)
{
    nextLevel = newLvl;
}

bool GSM::GameStateManager::ShouldExit()
{
    return currentLevel == nullptr;
}
