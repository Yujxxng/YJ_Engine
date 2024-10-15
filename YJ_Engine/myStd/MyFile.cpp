#include "MyFile.h"

#include <fstream>
#include <iostream>
#include "../Serializer/ComponentSerializer.h"
#include "../ComponentManager/GameObjectManager.h"

using json = nlohmann::json;

void MyFile::CreateDirectory(const char* path)
{
    std::cout << "FILE NOT FOUND " << path << std::endl;
    std::ofstream ofs(path);
    ofs.close();
}

json MyFile::LoadData(const char* path)
{
    std::ifstream jf(path);
    if (!jf.is_open())
    {
        MyFile::CreateDirectory(path);
        return nullptr;
    }

    using json = nlohmann::json;
    json data;
    jf.seekg(0, std::ios::end);
    if (jf.tellg() != 0) {
        jf.seekg(0);
        jf >> data;
    }
    jf.close();

    return data;
}

void MyFile::LoadObjectFile(const char* path)
{
    json objData = MyFile::LoadData(path);
    GameObjectManager::GetPtr()->LoadAllObjects(objData);
}
