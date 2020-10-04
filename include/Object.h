#pragma once

#include "doctest.h"
#include "Matrix.h"
#include "Vector.h"
#include "Point.h"
#include "Util.h"
#include "Material.h"
#include "Ray.h"
#include "Intersection.h"

class Object
{

protected:
    int ID;
    Matrix Transform;
    Point Origin;
    Material AMaterial;
    bool UseShadow;

public:
    Object();

    int GetID();

    Matrix GetTransform() const { return Transform; }
    Material GetMaterial() const { return AMaterial; }
    bool ShadowOn() const { return UseShadow; }

    inline void SetTransform(Matrix &M) { Transform = M; }
    inline void SetTransform(Matrix &&M) { Transform = M; }

    inline void SetMaterial(Material &M) { AMaterial = M; }
    inline void SetMaterial(Material &&M) { AMaterial = M; }

    inline void SetShadowOn(bool Shadow) { UseShadow = Shadow; }

    virtual Vector NormalAt(Point &P);
    inline virtual Vector NormalAt(Point &&P) { return NormalAt(P); }
    inline virtual Vector LocalNormalAt(Point &P) { return Vector(0.f, 0.f, 0.f); }
    inline virtual Vector LocalNormalAt(Point &&P) { return Vector(0.f, 0.f, 0.f); }

    inline virtual std::vector<Intersection<Object>> Intersect(const Ray &R)
    {
        return std::vector<Intersection<Object>>();
    }

    virtual std::vector<Intersection<Object>> Intersect(const Ray &R, std::shared_ptr<Object> &ObjectPtr);

    inline virtual std::vector<Intersection<Object>> LocalIntersect(const Ray &R, std::shared_ptr<Object> &ObjectPtr)
    {
        return std::vector<Intersection<Object>>();
    }
};

class TestShape : public Object
{

public:
    TestShape(int ID);
    TestShape();

    Ray SavedRay;

    virtual std::vector<Intersection<Object>> LocalIntersect(const Ray &R, std::shared_ptr<Object> &ObjectPtr) override;
    virtual Vector LocalNormalAt(Point &LocalPoint) override;
};