#pragma once

#include "doctest.h"
#include "Object.h"
#include "Util.h"
#include <memory>
#include <vector>

class Intersection
{
    float T;
    std::shared_ptr<Object> O;

public:
    Intersection();
    Intersection(float T, const Object &O);

    float GetT() const;
    std::shared_ptr<Object> GetObject() const;

    bool operator<(const Intersection &RHS) const { return T < RHS.GetT(); }
};

inline bool operator==(const Intersection &LHS, const Intersection &RHS)
{
    return Util::Equal(LHS.GetT(), RHS.GetT()) && LHS.GetObject()->GetID() == RHS.GetObject()->GetID();
}

inline bool operator!=(const Intersection &LHS, const Intersection &RHS)
{
    return (!(LHS == RHS));
}

std::ostream &operator<<(std::ostream &os, const Intersection &M);

std::shared_ptr<Intersection> Hit(std::vector<Intersection> &Intersections);

void Intersections(std::vector<Intersection> &I);
std::vector<Intersection> Intersections(std::vector<Intersection> &&I);