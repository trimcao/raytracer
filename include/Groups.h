#pragma once

#include "doctest.h"
#include "Object.h"
#include "Point.h"
#include "Vector.h"
#include "Intersection.h"
#include <vector>

class Groups : public Object
{
    std::vector<std::shared_ptr<Object>> Shapes;

public:
    Groups(int ID);
    Groups();

    int GetID();

    inline auto GetShapes() { return Shapes; }
    inline void SetShapes(std::vector<std::shared_ptr<Object>> &S) { Shapes = S; }

    virtual void AddChild(std::shared_ptr<Object> &S) override;
    virtual std::vector<Intersection<Object>> LocalIntersect(const Ray &LocalRay) override;
    virtual bool Include(Object *S) override;
};