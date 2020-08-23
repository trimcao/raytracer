#pragma once

class Vector : public Matrix
{
public:
    Vector();
    Vector(float X, float Y, float Z);

    inline float X() const { return this->At(0, 0); }
    inline float Y() const { return this->At(1, 0); }
    inline float Z() const { return this->At(2, 0); }

    Vector operator-() const { return Vector(-this->X(), -this->Y(), -this->Z()); }

    Vector operator*(float Scalar) const { return Vector(this->X() * Scalar, this->Y() * Scalar, this->Z() * Scalar); }

    Vector operator/(float Scalar) const { return Vector(this->X() / Scalar, this->Y() / Scalar, this->Z() / Scalar); }

    float Magnitude();
    Vector Normalize();

    float Dot(const Vector &V);
    Vector Cross(const Vector &V);
};

inline Vector operator-(const Vector &A, const Vector &B)
{
    return Vector(A.X() - B.X(), A.Y() - B.Y(), A.Z() - B.Z());
}

inline Vector operator+(const Vector &A, const Vector &B)
{
    return Vector(A.X() + B.X(), A.Y() + B.Y(), A.Z() + B.Z());
}

inline Vector operator*(float S, const Vector &B)
{
    return Vector(S * B.X(), S * B.Y(), S * B.Z());
}

std::ostream &operator<<(std::ostream &os, const Vector &V);