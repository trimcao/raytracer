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

public:
    Object();

    int GetID();

    Matrix GetTransform() const { return Transform; }
    Material GetMaterial() const { return AMaterial; }

    inline void SetTransform(Matrix &M) { Transform = M; }
    inline void SetTransform(Matrix &&M) { Transform = M; }

    inline void SetMaterial(Material &M) { AMaterial = M; }
    inline void SetMaterial(Material &&M) { AMaterial = M; }

    inline virtual Vector NormalAt(Point &P) { return Vector(0., 0., 0.); }
    inline virtual Vector NormalAt(Point &&P) { return NormalAt(P); }

    inline virtual std::vector<Intersection<Object>> Intersect(const Ray &R)
    {
        return std::vector<Intersection<Object>>();
    }

    inline virtual std::vector<Intersection<Object>> Intersect(const Ray &R, std::shared_ptr<Object> &ObjectPtr)
    {
        return std::vector<Intersection<Object>>();
    }

    inline virtual std::vector<float> LocalIntersect(const Ray &R)
    {
        return std::vector<float>();
    }
};