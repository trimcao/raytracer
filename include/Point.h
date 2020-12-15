#pragma once

// #include "doctest.h"
#include "Matrix.h"
#include "Vector.h"
#include "Util.h"

class Point : public Matrix
{
public:
    Point();
    Point(double X, double Y, double Z);
    Point(int NumRows, int NumCols);
    Point(Matrix &M);

    inline double X() const { return this->At(0, 0); }
    inline double Y() const { return this->At(1, 0); }
    inline double Z() const { return this->At(2, 0); }

    inline void SetX(double Val) { return this->Set(0, 0, Val); }
    inline void SetY(double Val) { return this->Set(1, 0, Val); }
    inline void SetZ(double Val) { return this->Set(2, 0, Val); }

    Point operator-() const { return Point(-this->X(), -this->Y(), -this->Z()); }

    Point operator*(double Scalar) const { return Point(this->X() * Scalar, this->Y() * Scalar, this->Z() * Scalar); }

    Point operator/(double Scalar) const { return Point(this->X() / Scalar, this->Y() / Scalar, this->Z() / Scalar); }

    void operator=(const Matrix &M)
    {
        if (M.GetNumRows() != 4 && M.GetNumCols() != 1)
            throw std::invalid_argument("size of a Point must be 4x1");
        if (!Util::Equal(M.At(3, 0), 1.))
            throw std::invalid_argument("W value must be 1.0");

        this->Set(0, 0, M.At(0, 0));
        this->Set(1, 0, M.At(1, 0));
        this->Set(2, 0, M.At(2, 0));
        this->Set(3, 0, M.At(3, 0));
    }
};

inline Vector operator-(const Point &A, const Point &B)
{
    return Vector(A.X() - B.X(), A.Y() - B.Y(), A.Z() - B.Z());
}

inline Point operator+(const Point &A, const Vector &B)
{
    return Point(A.X() + B.X(), A.Y() + B.Y(), A.Z() + B.Z());
}

inline Point operator+(const Vector &A, const Point &B)
{
    return Point(A.X() + B.X(), A.Y() + B.Y(), A.Z() + B.Z());
}

inline Point operator-(const Point &A, const Vector &B)
{
    return Point(A.X() - B.X(), A.Y() - B.Y(), A.Z() - B.Z());
}

inline Point operator*(double S, const Point &B)
{
    return Point(S * B.X(), S * B.Y(), S * B.Z());
}

std::ostream &operator<<(std::ostream &os, const Point &P);