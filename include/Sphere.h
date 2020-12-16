#pragma once

// #include "doctest.h"
#include "Object.h"
#include "Matrix.h"
#include "Vector.h"
#include "Point.h"
#include "Util.h"
#include "Material.h"
#include "Intersection.h"
#include <vector>

class Sphere : public Object
{

public:
    Sphere(int ID);
    Sphere();

    int GetID();

    virtual Vector LocalNormalAt(Point &LocalPoint) override;

    virtual std::vector<Intersection<Object>> Intersect(const Ray &R) override;
    virtual std::vector<Intersection<Object>> LocalIntersect(const Ray &LocalRay) override;

    static Sphere GlassSphere();

    virtual std::pair<Point, Point> BoundsOf() override;
};