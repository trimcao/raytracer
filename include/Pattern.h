#pragma once

#include "doctest.h"
#include "Color.h"
#include "Point.h"

class Pattern
{

public:
    static const Color Black;
    static const Color White;
    Pattern();
};

const Color Pattern::Black = Color(0., 0., 0.);
const Color Pattern::White = Color(1., 1., 1.);

class StripePattern : public Pattern
{
    Color A;
    Color B;

public:
    StripePattern(const Color &C1, const Color &C2);

    inline Color GetA() { return A; }
    inline Color GetB() { return B; }

    Color StripeAt(Point &P);
    inline Color StripeAt(Point &&P) { return StripeAt(P); }
};