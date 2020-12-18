#pragma once

#include "Point.h"
#include "Vector.h"
#include "Intersection.h"
#include "Ray.h"
#include "Intersection.h"
#include <vector>

class BoundingBoxes
{
public:
    Point Min, Max;

    BoundingBoxes();
    BoundingBoxes(Point Minimum, Point Maximum);
    BoundingBoxes(std::pair<Point, Point> MinMax);

    int GetID();

    void AddPoint(Point P);
    void AddBox(BoundingBoxes &B);

    bool ContainsPoint(Point P);
    bool ContainsBox(BoundingBoxes &B);

    BoundingBoxes Transform(Matrix &M);

    bool Intersect(const Ray &R);
};