#include "GraphicComponentManager.h"

GraphicComponentManager* GraphicComponentManager::graphic_ptr = nullptr;


GraphicComponentManager* GraphicComponentManager::getPtr()
{
    if (graphic_ptr == nullptr)
    {
        graphic_ptr = new GraphicComponentManager;

        return graphic_ptr;
    }
    else
        return graphic_ptr;
}

void GraphicComponentManager::DeletePtr()
{
    if (graphic_ptr != nullptr)
    {
        delete graphic_ptr;
        graphic_ptr = nullptr;
    }
}

void GraphicComponentManager::AddGraphic(GraphicComponent* lc)
{
    graphics.push_back(lc);
}

void GraphicComponentManager::DeleteGraphic(GraphicComponent* c)
{
    graphics.remove(c);
}

void GraphicComponentManager::Update()
{
    for (auto it = graphics.begin(); it != graphics.end(); it++)
    {
        (*it)->Update();
    }
}
