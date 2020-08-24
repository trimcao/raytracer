#pragma once

#include "doctest.h"
#include "Matrix.h"
#include "Util.h"

class Tuple : public Matrix
{

public:
    public:
    Tuple();
    Tuple(float X, float Y, float Z, float W);

    inline float X() const { return this->At(0, 0); }
    inline float Y() const { return this->At(1, 0); }
    inline float Z() const { return this->At(2, 0); }
    inline float W() const { return this->At(3, 0); }

    Tuple operator-() const { return Tuple(-this->X(), -this->Y(), -this->Z(), -this->W()); }

    Tuple operator*(float S) const { return Tuple(this->X() * S, this->Y() * S, this->Z() * S, this->W() * S); }

    Tuple operator/(float S) const { return Tuple(this->X() / S, this->Y() / S, this->Z() / S, this->W() / S); }
};

std::ostream &operator<<(std::ostream &os, const Tuple &T);
