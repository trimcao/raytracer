#include <iostream>
#include <exception>
#include <stdexcept>
#include "include/Object.h"

Object::Object()
{
    ID = 0;
}

int Object::GetID()
{
    return ID;
}

Vector Object::NormalAt(Point &P)
{
    return Vector(0.f, 0.f, 0.f);
}

Vector Object::NormalAt(Point &&P)
{
    return NormalAt(P);
}