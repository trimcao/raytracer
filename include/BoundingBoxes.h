#pragma once

#include "Object.h"
#include "Point.h"
#include "Vector.h"
#include "Intersection.h"
#include "Groups.h"
#include "CSG.h"
#include <vector>

class BoundingBoxes : public Object
{
    // std::vector<std::shared_ptr<Object>> Shapes;

public:
    Point Min, Max;

    BoundingBoxes(int ID);
    BoundingBoxes();
    BoundingBoxes(Point Minimum, Point Maximum, int ID=0);
    BoundingBoxes(std::pair<Point, Point> MinMax);

    int GetID();

    void AddPoint(Point P);
    void AddBox(BoundingBoxes &B);

    bool ContainsPoint(Point P);
    bool ContainsBox(BoundingBoxes &B);

    BoundingBoxes TransformBox(Matrix &M);

    static BoundingBoxes ParentSpaceBox(std::pair<Point, Point> ObjectBounds, Matrix Transformation);
    static BoundingBoxes BoundingBoxOf(Groups &G);
    static BoundingBoxes BoundingBoxOf(CSG &C);

    // inline auto GetShapes() { return Shapes; }
    // inline void SetShapes(std::vector<std::shared_ptr<Object>> &S) { Shapes = S; }

    // virtual void AddChild(std::shared_ptr<Object> &S) override;
    // virtual std::vector<Intersection<Object>> LocalIntersect(const Ray &LocalRay) override;
    // virtual bool Include(Object *S) override;
};