#pragma once

#include "doctest.h"
#include "Object.h"
#include "Matrix.h"
#include "Vector.h"
#include "Point.h"
#include "Util.h"

class Sphere : public Object
{

public:
    Sphere();
    Sphere(int ID);

    int GetID();

};