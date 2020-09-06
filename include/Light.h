#pragma once

#include "doctest.h"
// #include <iostream>
// #include "Util.h"
#include "Color.h"
#include "Point.h"

class Light
{
    Color Intensity;
    Point Position;

public:
    Light();
    Light(Color &C, Point &P);
    Light(Color &&C, Point &&P);

    inline Color GetIntensity() { return Intensity; }
    inline Point GetPosition() { return Position; }
    inline void SetIntensity(Color &C) { Intensity = C; }
    inline void SetPosition(Point &P) { Position = P; }
};