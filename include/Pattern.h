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

    virtual inline Color StripeAt(Point &P) { return Black; }
    virtual inline Color StripeAt(Point &&P) { return StripeAt(P); }

    // virtual inline Color StripeAtObject(std::shared_ptr<Object> &Obj, Point &P) { return Black; }
    // virtual inline Color StripeAtObject(std::shared_ptr<Object> &Obj, Point &&P) { return Black; }
};

class StripePattern : public Pattern
{
    Color A;
    Color B;

public:
    StripePattern(const Color &C1, const Color &C2);

    inline Color GetA() { return A; }
    inline Color GetB() { return B; }

    Color StripeAt(Point &P) override;
    inline Color StripeAt(Point &&P) override { return StripeAt(P); }

    // virtual Color StripeAtObject(std::shared_ptr<Object> &Obj, Point &P) override;
    // virtual inline Color StripeAtObject(std::shared_ptr<Object> &Obj, Point &&P) override { return StripeAtObject(Obj, P); }
};