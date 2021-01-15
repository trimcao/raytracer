#pragma once

// #include "doctest.h"
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

    virtual BoundingBoxes BoundsOf() override;
    virtual std::pair<std::vector<std::shared_ptr<Object>>, std::vector<std::shared_ptr<Object>>> PartitionChildren() override;
    virtual void MakeSubgroup(std::vector<std::shared_ptr<Object>> InShapes) override;
    virtual void Divide(int Threshold) override;
    inline virtual int GetCount() override { return Shapes.size(); }
    inline virtual std::vector<std::shared_ptr<Object>> GetChildren() override
    {
        return Shapes;
    }

    inline void SetMaterialChildren(Material &Mat) {
        for (auto &S: Shapes)
        {
            S->SetMaterial(Mat);
        }
    }
    inline void SetMaterialChildren(Material &&Mat) {
        SetMaterialChildren(Mat);
    }
};