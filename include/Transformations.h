#pragma once

// #include "doctest.h"
#include "Matrix.h"
#include "Vector.h"
#include "Point.h"
#include "Util.h"

class Transformations
{
public:
    static Matrix ViewTransform(Point &From, Point &To, Vector &Up);
    static Matrix ViewTransform(Point &&From, Point &&To, Vector &&Up);

    static Matrix Translation(double X, double Y, double Z);
    static Matrix Scaling(double X, double Y, double Z);
    static Matrix RotationX(double Rad);
    static Matrix RotationY(double Rad);
    static Matrix RotationZ(double Rad);
    // param XY means: how much we move X in proportion to Y
    static Matrix Shearing(double XY, double XZ, double YX, double YZ, double ZX, double ZY);
};