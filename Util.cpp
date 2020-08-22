#include "include/Util.h"
#include <cmath>


bool Util::Equal(const float &A, const float &B)
{
    if ( std::abs(A - B) <= EPSILON ) return true;
    return false;
}