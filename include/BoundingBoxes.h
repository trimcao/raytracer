#pragma once

#include "Object.h"
#include "Point.h"
#include "Vector.h"
#include "Intersection.h"
#include <vector>

class BoundingBoxes : public Object
{
    // std::vector<std::shared_ptr<Object>> Shapes;

public:
    Point Min, Max;

    BoundingBoxes(int ID);
    BoundingBoxes();
    BoundingBoxes(int ID, Point Minimum, Point Maximum);
    BoundingBoxes(std::pair<Point, Point> MinMax);

    int GetID();

    void AddPoint(Point P);

    // inline auto GetShapes() { return Shapes; }
    // inline void SetShapes(std::vector<std::shared_ptr<Object>> &S) { Shapes = S; }

    // virtual void AddChild(std::shared_ptr<Object> &S) override;
    // virtual std::vector<Intersection<Object>> LocalIntersect(const Ray &LocalRay) override;
    // virtual bool Include(Object *S) override;
};