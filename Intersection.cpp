#include <iostream>
#include <exception>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include <cmath>
#include "include/Intersection.h"
#include "include/Ray.h"
#include "include/Util.h"

template <class OT>
Intersection<OT>::Intersection() {}

template <class OT>
double Intersection<OT>::GetT() const
{
    return T;
}

template<class OT>
std::shared_ptr<OT> Intersection<OT>::GetObject() const
{
    return O;
}

template<class OT>
Intersection<OT>::Intersection(double T, const OT &O)
{
    this->T = T;
    this->O = std::make_shared<OT>(O);
}

template<class OT>
Intersection<OT>::Intersection(double T, std::shared_ptr<OT> &O)
{
    this->T = T;
    this->O = O;
}

template<class OT>
Intersection<OT>::Intersection(double T, std::shared_ptr<OT> &&O)
{
    this->T = T;
    this->O = O;
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
