#pragma once

#include "doctest.h"
#include "Ray.h"
#include "Util.h"
#include <memory>
#include <vector>

template <class ObjectType>
struct PreComputations
{
    std::shared_ptr<ObjectType> AObject;
    double T;
    Point Position;
    Point OverPosition;
    Vector EyeV;
    Vector NormalV;
    bool IsInside;
    Vector ReflectV;
    double N1;
    double N2;
};

template <class ObjectType>
class Intersection
{
    double T;
    std::shared_ptr<ObjectType> O;

public:
    Intersection();
    Intersection(double T, const ObjectType &O);
    Intersection(double T, std::shared_ptr<ObjectType> &O);
    Intersection(double T, std::shared_ptr<ObjectType> &&O);

    double GetT() const;
    std::shared_ptr<ObjectType> GetObject() const;

    bool operator<(const Intersection &RHS) const { return T < RHS.GetT(); }

    PreComputations<ObjectType> PrepareComputations(Ray &R,
        std::vector<Intersection<ObjectType>> IntersectionList = std::vector<Intersection<ObjectType>>{}
    );
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

// Important include here to make template works
#include "../Intersection.cpp"