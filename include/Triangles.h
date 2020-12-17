#pragma once

// #include "doctest.h"
#include "Object.h"
#include "Point.h"
#include "Vector.h"
#include "Intersection.h"
#include <vector>

class Triangles : public Object
{
    Point P1;
    Point P2;
    Point P3;
    Vector E1;
    Vector E2;
    Vector Normal;

public:
    Triangles(int ID);
    Triangles();
    Triangles(Point &&Point1, Point &&Point2, Point &&Point3);
    Triangles(Point &Point1, Point &Point2, Point &Point3);

    int GetID();
    inline Point GetP1() { return P1; }
    inline Point GetP2() { return P2; }
    inline Point GetP3() { return P3; }
    inline Vector GetE1() { return E1; }
    inline Vector GetE2() { return E2; }
    inline Vector GetNormal() { return Normal; }

    virtual Vector LocalNormalAt(Point &LocalPoint) override;
    virtual Vector LocalNormalAt(Point &&LocalPoint) override;

    virtual std::vector<Intersection<Object>> LocalIntersect(const Ray &LocalRay) override;
    virtual BoundingBoxes BoundsOf() override;
};

class SmoothTriangles : public Object
{
    Point P1;
    Point P2;
    Point P3;
    Vector E1;
    Vector E2;
    Vector N1;
    Vector N2;
    Vector N3;

public:
    SmoothTriangles(int ID);
    SmoothTriangles();
    SmoothTriangles(Point &&P1, Point &&P2, Point &&P3, Vector &&N1, Vector &&N2, Vector &&N3);
    SmoothTriangles(Point &P1, Point &P2, Point &P3, Vector &N1, Vector &N2, Vector &N3);

    int GetID();
    inline Point GetP1() { return P1; }
    inline Point GetP2() { return P2; }
    inline Point GetP3() { return P3; }
    inline Vector GetE1() { return E1; }
    inline Vector GetE2() { return E2; }
    inline Vector GetN1() { return N1; }
    inline Vector GetN2() { return N2; }
    inline Vector GetN3() { return N3; }
    // inline Vector GetNormal() { return Normal; }

    virtual Vector LocalNormalAt(Point &LocalPoint, Intersection<Object> &I) override;
    virtual Vector LocalNormalAt(Point &&LocalPoint, Intersection<Object> &I) override;

    virtual std::vector<Intersection<Object>> LocalIntersect(const Ray &LocalRay) override;
    virtual BoundingBoxes BoundsOf() override;
};