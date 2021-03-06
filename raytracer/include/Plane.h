#pragma once

// #include "doctest.h"
#include "Object.h"
#include "Vector.h"
#include "Intersection.h"
#include <vector>

class Plane : public Object
{

public:
    Plane(int ID);
    Plane();

    int GetID();

    virtual Vector LocalNormalAt(Point &LocalPoint) override;
    virtual Vector LocalNormalAt(Point &&LocalPoint) override;

    virtual std::vector<Intersection<Object>> LocalIntersect(const Ray &LocalRay) override;

    virtual BoundingBoxes BoundsOf() override;

    inline virtual std::shared_ptr<Object> Clone() override
    {
        return std::make_shared<Plane>(*this);
    }
};