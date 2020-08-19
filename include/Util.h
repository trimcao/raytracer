#pragma once

#include <cmath>

inline float EPSILON = 0.00001;

inline bool equal(const float A, const float B)
{
    if ( abs(A - B) <= EPSILON ) return true;
    return false;
}