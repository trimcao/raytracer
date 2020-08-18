// #define DOCTEST_CONFIG_DISABLE
#include "include/Vector.h"
#include <stdexcept>
#include "include/doctest.h"

Vector::Vector(int s)
{
    if (s<0)
        throw std::length_error("Vector constructor: negative size");
    elem = new double[s];
    sz = s;
}

double &Vector::operator[](int i)
{
    if (i<0 || size() <=i)
        throw std::out_of_range("Vector:operator[]");
    return elem[i];
}

int Vector::size()
{
    return sz;
}

TEST_CASE("testing the Vector")
{
    Vector v = Vector(6);
    CHECK(v.size() == 6);
}