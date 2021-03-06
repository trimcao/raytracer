#pragma once

// #include "doctest.h"
#include "Object.h"
#include "Point.h"
#include "Vector.h"
#include "Intersection.h"
#include "BoundingBoxes.h"
#include <vector>

class Cones : public Object
{
    double Min;
    double Max;
    bool Closed;

public:
    Cones(int ID);
    Cones();
    Cones(double Minimum, double Maximum, bool IsClosed=false);

    int GetID();

    virtual Vector LocalNormalAt(Point &LocalPoint) override;
    virtual Vector LocalNormalAt(Point &&LocalPoint) override;

    virtual std::vector<Intersection<Object>> LocalIntersect(const Ray &LocalRay) override;

    inline double GetMin() { return Min; }
    inline double GetMax() { return Max; }
    inline bool IsClosed() { return Closed; }

    inline void SetMin(double M) { Min = M; }
    inline void SetMax(double M) { Max = M; }
    inline void SetClosed(bool IsClosed) { Closed = IsClosed; }

    virtual BoundingBoxes BoundsOf() override;

    inline virtual std::shared_ptr<Object> Clone() override
    {
        return std::make_shared<Cones>(*this);
    }

private:
    // a helper function to reduce duplication.
    // checks to see if the intersection at 't' is within a radius
    // of abs(Y) (the radius of your Cones) from the y axis
    bool CheckCap(const Ray &R, double T, double AbsY);

    void IntersectCaps(const Ray &R, std::vector<Intersection<Object>> &Intersections);
};

std::vector<double> CheckAxis(double Origin, double Direction);

