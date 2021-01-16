#pragma once

// #include "doctest.h"
#include "Matrix.h"
#include "Vector.h"
#include "Point.h"
#include "Util.h"
#include "Material.h"
#include "Ray.h"
#include "Intersection.h"
#include "BoundingBoxes.h"
#include <limits>

class Object
{

protected:
    int ID;
    Matrix Transform;
    Matrix TransformInverse;
    Point Origin;
    Material AMaterial;
    bool UseShadow;
    Object *Parent;
    BoundingBoxes BoxCache;
    bool IsBoundingBoxCached = false;

public:
    Object();

    int GetID();

    Matrix GetTransform() const { return Transform; }
    Matrix GetTransformInverse() const { return TransformInverse; }
    Material GetMaterial() const { return AMaterial; }
    bool ShadowOn() const { return UseShadow; }
    Object *GetParent() { return Parent; }

    void SetTransform(Matrix &M);
    inline void SetTransform(Matrix &&M) { SetTransform(M); }

    inline virtual void SetMaterial(Material &M) { AMaterial = M; }
    inline virtual void SetMaterial(Material &&M) { SetMaterial(M); }

    inline void SetParent(Object *P) { Parent = P; }

    inline void SetShadowOn(bool Shadow) { UseShadow = Shadow; }

    virtual Vector NormalAt(Point &P);
    virtual Vector NormalAt(Point &P, Intersection<Object> &I);
    inline virtual Vector NormalAt(Point &&P) { return NormalAt(P); }
    inline virtual Vector NormalAt(Point &&P, Intersection<Object> &I) { return NormalAt(P, I); }
    inline virtual Vector LocalNormalAt(Point &P) { return Vector(0.f, 0.f, 0.f); }
    inline virtual Vector LocalNormalAt(Point &&P) { return Vector(0.f, 0.f, 0.f); }
    inline virtual Vector LocalNormalAt(Point &P, Intersection<Object> &I) { return LocalNormalAt(P); }
    inline virtual Vector LocalNormalAt(Point &&P, Intersection<Object> &I) { return LocalNormalAt(P); }

    virtual std::vector<Intersection<Object>> Intersect(const Ray &R);

    inline virtual std::vector<Intersection<Object>> LocalIntersect(const Ray &R)
    {
        return std::vector<Intersection<Object>>();
    }

    inline virtual void AddChild(std::shared_ptr<Object> &S) {};
    inline virtual bool Include(Object *S) { return (this == S); }

    inline virtual BoundingBoxes BoundsOf() { return BoundingBoxes(); }
    inline virtual BoundingBoxes ParentSpaceBoundsOf() { return BoundsOf().Transform(Transform); }

    // divide group into subgroups
    inline virtual std::pair<std::vector<std::shared_ptr<Object>>, std::vector<std::shared_ptr<Object>>> PartitionChildren()
    {
        std::vector<std::shared_ptr<Object>> Left {};
        std::vector<std::shared_ptr<Object>> Right {};
        return std::pair<std::vector<std::shared_ptr<Object>>, std::vector<std::shared_ptr<Object>>> {Left, Right};
    }

    inline virtual void MakeSubgroup(std::vector<std::shared_ptr<Object>> InShapes) {}
    inline virtual void Divide(int Threshold) {}
    inline virtual int GetCount() { return 1; }
    inline virtual std::vector<std::shared_ptr<Object>> GetChildren()
    {
        return std::vector<std::shared_ptr<Object>> {};
    }

};

class TestShape : public Object
{

public:
    TestShape(int ID);
    TestShape();

    std::unique_ptr<Ray> SavedRay;

    virtual std::vector<Intersection<Object>> LocalIntersect(const Ray &R) override;
    virtual Vector LocalNormalAt(Point &LocalPoint) override;
    virtual BoundingBoxes BoundsOf() override;
};