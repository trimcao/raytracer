#pragma once

#include <limits>

namespace Util
{
    const double EPSILON = 0.00001;
    const double Inf = std::numeric_limits<double>::max();

    bool Equal(const double &A, const double &B);
}
