#include <iostream>
#include <exception>
#include <stdexcept>
#include "include/Intersection.h"
#include "include/Sphere.h"
#include "include/Util.h"

Intersection::Intersection() {}

float Intersection::GetT()
{
    return T;
}
std::shared_ptr<Object> Intersection::GetObject()
{
    return O;
}

Intersection::Intersection(float T, const Object &O)
{
    this->T = T;
    this->O = std::make_shared<Object>(O);
}

TEST_CASE("An intersection encapsulates t and object")
{
    Sphere S(3);
    Intersection I(3.5f, S);

    CHECK(Util::Equal(I.GetT(), 3.5f));
    CHECK(I.GetObject()->GetID() == 3);
}