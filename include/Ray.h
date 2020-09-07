#pragma once

#include "doctest.h"
#include "Matrix.h"
#include "Vector.h"
#include "Point.h"
#include "Util.h"
#include "Sphere.h"
#include <vector>

class Ray
{
    Point Origin;
    Vector Direction;

public:
    Ray();
    Ray(Point &&O, Vector &&D);
    Ray(Point &O, Vector &&D);
    Ray(Point &O, Vector &D);

    Point GetOrigin() const;
    Vector GetDirection() const;

    Point Position(float T);

    Ray Transform(Matrix &M) const;
    Ray Transform(Matrix &&M) const;
};
