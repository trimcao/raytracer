#pragma once

#include "doctest.h"
#include "Ray.h"
#include "Object.h"
#include "Sphere.h"
#include "Util.h"
#include <memory>
#include <vector>

template <class ObjectType>
class Intersection
{
    float T;
    std::shared_ptr<ObjectType> O;

public:
    Intersection();
    Intersection(float T, const ObjectType &O);

    float GetT() const;
    std::shared_ptr<ObjectType> GetObject() const;

    bool operator<(const Intersection &RHS) const { return T < RHS.GetT(); }
};

template <class OT>
inline bool operator==(const Intersection<OT> &LHS, const Intersection<OT> &RHS)
{
    return Util::Equal(LHS.GetT(), RHS.GetT()) && LHS.GetObject()->GetID() == RHS.GetObject()->GetID();
}

template <class OT>
inline bool operator!=(const Intersection<OT> &LHS, const Intersection<OT> &RHS)
{
    return (!(LHS == RHS));
}

template <class OT>
std::ostream &operator<<(std::ostream &os, const Intersection<OT> &M);

template <class OT>
std::shared_ptr<Intersection<OT>> Hit(std::vector<Intersection<OT>> &Intersections);

template <class OT>
void Intersections(std::vector<Intersection<OT>> &I);

template <class OT>
std::vector<Intersection<OT>> Intersections(std::vector<Intersection<OT>> &&I);

std::vector<Intersection<Sphere>> Intersect(const Ray &R, const Sphere &S);
