#pragma once

#include "doctest.h"
#include "Matrix.h"
#include "Vector.h"
#include "Point.h"
#include "Util.h"

class Transformations
{
public:
    static Matrix ViewTransform(Point &From, Point &To, Vector &Up);
    static Matrix ViewTransform(Point &&From, Point &&To, Vector &&Up);
};