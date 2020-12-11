#pragma once

#include "doctest.h"
#include "Ray.h"
#include "Util.h"
#include <memory>
#include <vector>

template <class ObjectType>
struct PreComputations
{
    ObjectType *AObject;
    double T;
    Point Position;
    Point OverPosition;
    Point UnderPosition;
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
    double U;
    double V;
    ObjectType *O;

public:
    Intersection();
    Intersection(double T, ObjectType &O);
    Intersection(double T, ObjectType *O);
    Intersection(double T, ObjectType *O, double U, double V);

    double GetT() const;
    double GetU() const;
    double GetV() const;
    ObjectType *GetObject() const;

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

// Template definitions must live in the header file for the project to compile
// Otherwise, I will need to explicitly declare all the cases the template will be used for.

template <class OT>
Intersection<OT>::Intersection() {}

template <class OT>
double Intersection<OT>::GetT() const
{
    return T;
}

template <class OT>
double Intersection<OT>::GetU() const
{
    return U;
}

template <class OT>
double Intersection<OT>::GetV() const
{
    return V;
}

template<class OT>
OT *Intersection<OT>::GetObject() const
{
    return O;
}

template<class OT>
Intersection<OT>::Intersection(double T, OT &O)
{
    this->T = T;
    this->O = &O;
}

template<class OT>
Intersection<OT>::Intersection(double T, OT *O)
{
    this->T = T;
    this->O = O;
}

template<class OT>
Intersection<OT>::Intersection(double T, OT *O, double U, double V)
{
    this->T = T;
    this->O = O;
    this->U = U;
    this->V = V;
}

template<class OT>
std::ostream &operator<<(std::ostream &os, const Intersection<OT> &I)
{
    os << "Intersection{ " << I.GetT() << ", ";

    if (I.GetObject())
        os << I.GetObject()->GetID() << " }";
    else
        os << " }";

    return os;
}

template<class OT>
std::vector<Intersection<OT>> Intersections(std::vector<Intersection<OT>> &&I)
{
    std::sort(I.begin(), I.end());
    return I;
}

template<class OT>
void Intersections(std::vector<Intersection<OT>> &I)
{
    std::sort(I.begin(), I.end());
}

template<class OT>
std::shared_ptr<Intersection<OT>> Hit(std::vector<Intersection<OT>> &Intersections)
{
    for (auto &I : Intersections)
    {
        if (I.GetT() > 0.)
            return std::make_shared<Intersection<OT>>(I);
    }
    return nullptr;
}

template<class OT>
PreComputations<OT> Intersection<OT>::PrepareComputations(Ray &R, std::vector<Intersection<OT>> IntersectionList)
{
    if (IntersectionList.size() == 0)
        IntersectionList.push_back(*this);

    PreComputations<OT> Comps;
    Comps.T = T;
    Comps.AObject = O;
    Comps.Position = R.Position(T);
    Comps.EyeV = -R.GetDirection();
    Comps.NormalV = Comps.AObject->NormalAt(Comps.Position);
    Comps.IsInside = false;

    if (Comps.NormalV.Dot(Comps.EyeV) < 0.)
    {
        Comps.IsInside = true;
        Comps.NormalV = -Comps.NormalV;
    }

    Comps.OverPosition = Comps.Position + Comps.NormalV * Util::EPSILON;

    Comps.ReflectV = R.GetDirection().Reflect(Comps.NormalV);

    Comps.N1 = 1.;
    Comps.N2 = 1.;

    return Comps;
}