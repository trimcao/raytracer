#pragma once

#include "doctest.h"
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

    virtual Vector NormalAt(Point &P) override;
    virtual Vector NormalAt(Point &&P) override;

    virtual std::vector<Intersection<Object>> Intersect(const Ray &R) override;
    virtual std::vector<Intersection<Object>> Intersect(const Ray &R, std::shared_ptr<Object> &ObjectPtr) override;
    virtual std::vector<float> LocalIntersect(const Ray &R) override;
};