#pragma once

#include "doctest.h"
#include <vector>
#include <memory>
#include "Light.h"
#include "Object.h"

class World
{
    std::shared_ptr<Light> ALight;
    std::vector<std::shared_ptr<Object>> Objects;

public:
    World();
    World(Light &NewLight, std::vector<std::shared_ptr<Object>> &NewObjects);
    World(Light &&NewLight, std::vector<std::shared_ptr<Object>> &&NewObjects);
    static World DefaultWorld();

    inline void SetLight(Light &NewLight) { ALight = std::make_shared<Light>(NewLight); }
    inline void SetLight(Light &&NewLight) { SetLight(NewLight); };
    inline void AddObject(Object &NewObject) { Objects.push_back(std::make_shared<Object>(NewObject)); }
    inline void AddObject(Object &&NewObject) { AddObject(NewObject); }

    inline std::shared_ptr<Light> GetLight() const { return ALight; }
    inline std::shared_ptr<Object> GetObjectAt(int Idx) const { return Objects[Idx]; }
    inline std::vector<std::shared_ptr<Object>> GetObjects() const { return Objects; }
};