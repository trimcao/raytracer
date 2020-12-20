#include "include/Util.h"
#include <cmath>


bool Util::Equal(const double &A, const double &B)
{
    if ( std::abs(A - B) <= EPSILON ) return true;
    return false;
}