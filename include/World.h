#pragma once

// #include "doctest.h"
#include <vector>
#include <memory>
#include "Light.h"
#include "Object.h"
#include "Intersection.h"
#include "Color.h"

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

    template <class Derived>
    inline void AddObject(Derived &NewObject) { Objects.push_back(std::make_shared<Derived>(NewObject)); }
    template <class Derived>
    void AddObject(std::shared_ptr<Derived> &NewObjectPtr);

    inline void AddObject(std::shared_ptr<Object> &NewObjectPtr) { Objects.push_back(NewObjectPtr); }

    inline std::shared_ptr<Light> GetLight() const { return ALight; }
    inline std::shared_ptr<Object> GetObjectAt(int Idx) const { return Objects[Idx]; }
    inline std::vector<std::shared_ptr<Object>> GetObjects() const { return Objects; }

    std::vector<Intersection<Object>> Intersect(const Ray &R);
    std::vector<Intersection<Object>> Intersect(const Ray &R, std::shared_ptr<Object> &ObjectPtr);

    Color ShadeHit(PreComputations<Object> &Comps, bool RenderShadow, int Remaining);

    Color ColorAt(Ray &R, bool RenderShadow, int Remaining);

    bool IsShadowed(Point &P);

    Color ReflectedColor(PreComputations<Object> &Comps, bool RenderShadow, int Remaining);
    Color RefractedColor(PreComputations<Object> &Comps, bool RenderShadow=true, int Remaining=5);
};