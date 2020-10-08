#pragma once

#include "doctest.h"
#include "Object.h"
#include "Point.h"
#include "Vector.h"
#include "Intersection.h"
#include <vector>

class Cylinders : public Object
{

public:
    Cylinders(int ID);
    Cylinders();

    int GetID();

    virtual Vector LocalNormalAt(Point &LocalPoint) override;
    virtual Vector LocalNormalAt(Point &&LocalPoint) override;

    virtual std::vector<Intersection<Object>> LocalIntersect(const Ray &LocalRay, std::shared_ptr<Object> &ObjectPtr) override;
};

std::vector<double> CheckAxis(double Origin, double Direction);