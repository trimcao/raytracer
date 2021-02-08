#pragma once

// #include "doctest.h"
#include "Object.h"
#include "Point.h"
#include "Vector.h"
#include "Intersection.h"
#include <vector>

enum class CSGOp
{
    Union,
    Intersection,
    Difference
};

class CSG : public Object
{
    std::shared_ptr<Object> Left;
    std::shared_ptr<Object> Right;
    CSGOp Operator;

public:
    CSG(int ID);
    CSG();
    CSG(CSGOp Op, std::shared_ptr<Object> &L, std::shared_ptr<Object> &R);

    int GetID();

    inline auto GetLeft() { return Left; }
    inline auto GetRight() { return Right; }
    inline auto GetOp() { return Operator; }

    virtual std::vector<Intersection<Object>> LocalIntersect(const Ray &LocalRay) override;

    bool IntersectionAllowed(bool LHit, bool InL, bool InR);
    std::vector<Intersection<Object>> FilterIntersections(std::vector<Intersection<Object>> &XS);

    virtual bool Include(Object *S) override;

    virtual BoundingBoxes BoundsOf() override;

    inline virtual std::shared_ptr<Object> Clone() override
    {
        return std::make_shared<CSG>(*this);
    }
};