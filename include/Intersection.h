#pragma once

#include "doctest.h"
#include "Object.h"
#include <memory>
#include <vector>

class Intersection
{
    float T;
    std::shared_ptr<Object> O;

public:
    Intersection();
    Intersection(float T, const Object &O);

    float GetT();
    std::shared_ptr<Object> GetObject();
};
