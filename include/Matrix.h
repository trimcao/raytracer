#pragma once

// #include "doctest.h"
#include <iostream>
#include <vector>

class Matrix
{
private:
    int numRows, numCols;
    std::vector<std::vector<double>> m;

public:
    Matrix();
    Matrix(int NumRows, int NumCols, double Val);
    Matrix(int NumRows, int NumCols);

    // create a 4x1 Tuple
    Matrix(double X, double Y, double Z, double W);

    double &operator()(const int R, const int C) { return m[R][C]; }

    inline double At(const int R, const int C) const { return m[R][C]; }
    inline void Set(const int R, const int C, double Val) { m[R][C] = Val; }

    Matrix operator*(double S) const
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

    Matrix operator-() const { return (*this) * (-1.); }

    Matrix operator/(double S) const { return (*this) * (1.f / S); }

    inline bool IsValid(int Row, int Col) const
    {
        return Row >= 0 && Row < numRows && Col >= 0 && Col < numCols;
    }

    template<class Derived>
    Derived Mul(const Derived &RHS);

    // transpose
    Matrix T();

    double Determinant();

    Matrix Submatrix(int RowRemoved, int ColRemoved);

    double Minor(int Row, int Col);

    double Cofactor(int Row, int Col);

    bool IsInvertible();

    Matrix Inverse();

    int GetNumRows() const;
    int GetNumCols() const;

    static Matrix Identity(int Size);
    static Matrix Identity();

    static Matrix Translation(double X, double Y, double Z);
    static Matrix Scaling(double X, double Y, double Z);
    static Matrix RotationX(double Rad);
    static Matrix RotationY(double Rad);
    static Matrix RotationZ(double Rad);
    
    // param XY means: how much we move X in proportion to Y
    static Matrix Shearing(double XY, double XZ, double YX, double YZ, double ZX, double ZY);

    Matrix Translate(double X, double Y, double Z);
    Matrix Scale(double X, double Y, double Z);
    Matrix RotateX(double Rad);
    Matrix RotateY(double Rad);
    Matrix RotateZ(double Rad);
    Matrix Shear(double XY, double XZ, double YX, double YZ, double ZX, double ZY);

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

template<class Derived>
Derived Matrix::Mul(const Derived &RHS)
{
    static_assert(std::is_base_of<Matrix, Derived>::value, "Derived not derived from Matrix");

    if (this->GetNumCols() != RHS.GetNumRows())
        throw std::invalid_argument("LHS matrix's rows must match RHS matrix's columns.");

    int Dim = this->GetNumCols();

    Derived Res(this->GetNumRows(), RHS.GetNumCols());
    for (int r = 0; r < Res.GetNumRows(); ++r)
    {
        for (int c = 0; c < Res.GetNumCols(); ++c)
        {
            for (int i = 0; i < Dim; ++i)
            {
                Res(r, c) += this->At(r, i) * RHS.At(i, c);
            }
        }
    }
    return Res;
}