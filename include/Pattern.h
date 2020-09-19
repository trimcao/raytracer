#pragma once

#include "doctest.h"
#include "Color.h"
#include "Point.h"
#include "Matrix.h"

class Object;

class Pattern
{

protected:
    Matrix Transform;

public:
    static const Color Black;
    static const Color White;

    Pattern();

    inline Matrix GetTransform() { return Transform; }
    inline void SetTransform(Matrix &T) { Transform = T; }
    inline void SetTransform(Matrix &&T) { Transform = T; }

    virtual inline Color PatternAt(Point &P) { return Black; }
    virtual inline Color PatternAt(Point &&P) { return PatternAt(P); }

};

class TestPattern : public Pattern
{

public:
    TestPattern();

    Color PatternAt(Point &P) override;
    inline Color PatternAt(Point &&P) override { return PatternAt(P); }

};

class StripePattern : public Pattern
{
    Color A;
    Color B;

public:
    StripePattern(const Color &C1, const Color &C2);

    inline Color GetA() { return A; }
    inline Color GetB() { return B; }

    Color PatternAt(Point &P) override;
    inline Color PatternAt(Point &&P) override { return PatternAt(P); }

};