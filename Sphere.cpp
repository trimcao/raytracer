#include <iostream>
#include <exception>
#include <stdexcept>
#include "include/Sphere.h"

Sphere::Sphere() {}

Sphere::Sphere(int ID)
{
    this->ID = ID;
}

int Sphere::GetID()
{
    return ID;
}