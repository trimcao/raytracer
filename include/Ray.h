#pragma once

#include "doctest.h"
#include "Matrix.h"
#include "Vector.h"
#include "Point.h"
#include "Util.h"
#include "Sphere.h"
#include "Intersection.h"
#include <vector>

class Ray
{
    Point Origin;
    Vector Direction;

public:
    Ray();
    Ray(Point &&O, Vector &&D);

    Point GetOrigin() const;
    Vector GetDirection() const;

    Point Position(float T);
};

std::vector<Intersection> Intersect(const Ray &R, const Sphere &S);