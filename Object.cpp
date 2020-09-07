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
