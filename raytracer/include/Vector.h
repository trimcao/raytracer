#pragma once

// #include "doctest.h"
#include <iostream>
#include "Util.h"

class Vector : public Matrix
{
public:
    Vector();
    Vector(double X, double Y, double Z);
    Vector(int NumRows, int NumCols);
    Vector(Matrix &M);
    Vector(Matrix &&M);

    inline double X() const { return this->At(0, 0); }
    inline double Y() const { return this->At(1, 0); }
    inline double Z() const { return this->At(2, 0); }

    inline void SetW(double Val) { this->Set(3, 0, Val); }

    Vector operator-() const { return Vector(-this->X(), -this->Y(), -this->Z()); }

    Vector operator*(double Scalar) const { return Vector(this->X() * Scalar, this->Y() * Scalar, this->Z() * Scalar); }

    Vector operator/(double Scalar) const { return Vector(this->X() / Scalar, this->Y() / Scalar, this->Z() / Scalar); }

    void operator=(const Matrix &M)
    {
        if (M.GetNumRows() != 4 && M.GetNumCols() != 1)
            throw std::invalid_argument("size of a Vector must be 4x1");
        if (!Util::Equal(M.At(3, 0), 0.))
            throw std::invalid_argument("W value must be 0.0");

        this->Set(0, 0, M.At(0, 0));
        this->Set(1, 0, M.At(1, 0));
        this->Set(2, 0, M.At(2, 0));
        this->Set(3, 0, M.At(3, 0));
    }

    double Magnitude();
    Vector Normalize();

    double Dot(const Vector &V);
    Vector Cross(const Vector &V);

    Vector Reflect(const Vector &N);
};

inline Vector operator-(const Vector &A, const Vector &B)
{
    return Vector(A.X() - B.X(), A.Y() - B.Y(), A.Z() - B.Z());
}

inline Vector operator+(const Vector &A, const Vector &B)
{
    return Vector(A.X() + B.X(), A.Y() + B.Y(), A.Z() + B.Z());
}

inline Vector operator*(double S, const Vector &B)
{
    return Vector(S * B.X(), S * B.Y(), S * B.Z());
}

std::ostream &operator<<(std::ostream &os, const Vector &V);