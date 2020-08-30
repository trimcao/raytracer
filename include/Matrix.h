#pragma once

#include "doctest.h"
#include <iostream>
#include <vector>

class Matrix
{
private:
    int numRows, numCols;
    std::vector<std::vector<float>> m;

public:
    Matrix();
    Matrix(int NumRows, int NumCols, float Val);
    Matrix(int NumRows, int NumCols);

    // create a 4x1 Tuple
    Matrix(float X, float Y, float Z, float W);

    float &operator()(const int R, const int C) { return m[R][C]; }

    inline float At(const int R, const int C) const { return m[R][C]; }
    inline void Set(const int R, const int C, float Val) { m[R][C] = Val; }

    Matrix operator*(float S) const
    {
        Matrix Res = Matrix(numRows, numCols);
        for (int r = 0; r < numRows; ++r)
        {
            for (int c = 0; c < numCols; ++c)
            {
                Res(r, c) = m[r][c] * S;
            }
        }
        return Res;
    }

    Matrix operator-() const { return (*this) * (-1.f); }

    Matrix operator/(float S) const { return (*this) * (1.f / S); }

    inline bool IsValid(int Row, int Col) const
    {
        return Row >= 0 && Row < numRows && Col >= 0 && Col < numCols;
    }

    template<class Derived>
    Derived Mul(const Derived &RHS);

    // transpose
    Matrix T();

    float Determinant();

    Matrix Submatrix(int RowRemoved, int ColRemoved);

    float Minor(int Row, int Col);

    float Cofactor(int Row, int Col);

    bool IsInvertible();

    Matrix Inverse();

    int GetNumRows() const;
    int GetNumCols() const;

    static Matrix Identity(int Size);

    static Matrix Translation(float X, float Y, float Z);
    static Matrix Scaling(float X, float Y, float Z);
    static Matrix RotationX(float Rad);
    static Matrix RotationY(float Rad);
    static Matrix RotationZ(float Rad);
    
    // param XY means: how much we move X in proportion to Y
    static Matrix Shearing(float XY, float XZ, float YX, float YZ, float ZX, float ZY);

    Matrix Translate(float X, float Y, float Z);
    Matrix Scale(float X, float Y, float Z);
    Matrix RotateX(float Rad);
    Matrix RotateY(float Rad);
    Matrix RotateZ(float Rad);
    Matrix Shear(float XY, float XZ, float YX, float YZ, float ZX, float ZY);

};

bool operator==(const Matrix &LHS, const Matrix &RHS);

inline bool operator!=(const Matrix &LHS, const Matrix &RHS)
{
    return (!(LHS == RHS));
}

Matrix operator+(const Matrix &LHS, const Matrix &RHS);

Matrix operator-(const Matrix &LHS, const Matrix &RHS);

Matrix operator*(const Matrix &LHS, const Matrix &RHS);

Matrix operator/(const Matrix &LHS, const Matrix &RHS);

std::ostream &operator<<(std::ostream &os, const Matrix &M);