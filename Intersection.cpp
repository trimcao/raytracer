#include <iostream>
#include <exception>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include "include/Intersection.h"
#include "include/Sphere.h"
#include "include/Util.h"

Intersection::Intersection() {}

float Intersection::GetT() const
{
    return T;
}
std::shared_ptr<Object> Intersection::GetObject() const
{
    return O;
}

Intersection::Intersection(float T, const Object &O)
{
    this->T = T;
    this->O = std::make_shared<Object>(O);
}

std::ostream &operator<<(std::ostream &os, const Intersection &I)
{
    os << "Intersection{ " << I.GetT() << ", ";

    if (I.GetObject())
        os << I.GetObject()->GetID() << " }";
    else
        os << " }";

    return os;
}

std::vector<Intersection> Intersections(std::vector<Intersection> &&I)
{
    std::sort(I.begin(), I.end());
    return I;
}

void Intersections(std::vector<Intersection> &I)
{
    std::sort(I.begin(), I.end());
}

std::shared_ptr<Intersection> Hit(std::vector<Intersection> &Intersections)
{
    for (auto I : Intersections)
    {
        if (I.GetT() > 0.f)
            return std::make_shared<Intersection>(I);
    }
    return nullptr;
}

TEST_CASE("An intersection encapsulates t and object")
{
    Sphere S(3);
    Intersection I(3.5f, S);

    CHECK(Util::Equal(I.GetT(), 3.5f));
    CHECK(I.GetObject()->GetID() == 3);
}

TEST_CASE("The hit, when all intersections have positive t")
{
    Sphere S(1);
    Intersection I1(1.f, S);
    Intersection I2(2.f, S);
    auto XS = Intersections(std::vector<Intersection> {I1, I2});

    CHECK(*Hit(XS) == I1);
}

TEST_CASE("The hit, when some intersections have negative t")
{
    Sphere S(1);
    Intersection I1(-1.f, S);
    Intersection I2(1.f, S);
    auto XS = Intersections(std::vector<Intersection> {I1, I2});

    CHECK(*Hit(XS) == I2);
}

TEST_CASE("The hit, when all intersections have negative t")
{
    Sphere S(1);
    Intersection I1(-2.f, S);
    Intersection I2(-1.f, S);
    auto XS = Intersections(std::vector<Intersection> {I1, I2});

    CHECK(Hit(XS) == nullptr);
}

TEST_CASE("The hit, when all intersections have negative t")
{
    Sphere S(1);
    Intersection I1(5.f, S);
    Intersection I2(7.f, S);
    Intersection I3(-3.f, S);
    Intersection I4(2.f, S);
    auto XS = Intersections(std::vector<Intersection> {I1, I2, I3, I4});

    CHECK(*Hit(XS) == I4);
}