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
    Matrix TransformInverse;
    Point Origin;
    Material AMaterial;
    bool UseShadow;
    Object *Parent;

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

    inline void SetMaterial(Material &M) { AMaterial = M; }
    inline void SetMaterial(Material &&M) { AMaterial = M; }

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
};

class TestShape : public Object
{

public:
    TestShape(int ID);
    TestShape();

    Ray SavedRay;

    virtual std::vector<Intersection<Object>> LocalIntersect(const Ray &R) override;
    virtual Vector LocalNormalAt(Point &LocalPoint) override;
};