#pragma once

// #include "doctest.h"
#include "Matrix.h"
#include "Util.h"

class Tuple : public Matrix
{

public:
    public:
    Tuple();
    Tuple(double X, double Y, double Z, double W);
    Tuple(int NumRows, int NumCols);

    inline double X() const { return this->At(0, 0); }
    inline double Y() const { return this->At(1, 0); }
    inline double Z() const { return this->At(2, 0); }
    inline double W() const { return this->At(3, 0); }

    Tuple operator-() const { return Tuple(-this->X(), -this->Y(), -this->Z(), -this->W()); }

    Tuple operator*(double S) const { return Tuple(this->X() * S, this->Y() * S, this->Z() * S, this->W() * S); }

    Tuple operator/(double S) const { return Tuple(this->X() / S, this->Y() / S, this->Z() / S, this->W() / S); }

    void operator=(const Matrix &M)
    {
        if (M.GetNumRows() != 4 && M.GetNumCols() != 1)
            throw std::invalid_argument("size of a Tuple must be 4x1");

        this->Set(0, 0, M.At(0, 0));
        this->Set(1, 0, M.At(1, 0));
        this->Set(2, 0, M.At(2, 0));
        this->Set(3, 0, M.At(3, 0));
    }
};

std::ostream &operator<<(std::ostream &os, const Tuple &T);
