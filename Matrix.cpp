#include <iostream>
#include <exception>
#include <stdexcept>
#include <vector>
#include <string>
#include "include/Util.h"
#include "include/Matrix.h"
#include "include/Point.h"
#include "include/Tuple.h"

Matrix::Matrix() {}

Matrix::Matrix(int NumRows, int NumCols, float Val)
{
    numRows = NumRows;
    numCols = NumCols;
    m = std::vector<std::vector<float>>(numRows, std::vector<float>(numCols, Val));
}

Matrix::Matrix(int NumRows, int NumCols) : Matrix(NumRows, NumCols, 0.f)
{
}

Matrix::Matrix(float X, float Y, float Z, float W) : Matrix(4, 1, 0.f)
{
    m[0][0] = X;
    m[1][0] = Y;
    m[2][0] = Z;
    m[3][0] = W;
}

int Matrix::GetNumRows() const
{
    return numRows;
}

int Matrix::GetNumCols() const
{
    return numCols;
}

bool operator==(const Matrix &LHS, const Matrix &RHS)
{
    if (LHS.GetNumRows() != RHS.GetNumRows() || LHS.GetNumCols() != RHS.GetNumCols())
        return false;

    for (int r = 0; r < LHS.GetNumRows(); ++r)
    {
        for (int c = 0; c < LHS.GetNumCols(); ++c)
        {
            if (!Util::Equal(LHS.At(r, c), RHS.At(r, c)))
                return false;
        }
    }
    return true;
}

std::ostream &operator<<(std::ostream &os, const Matrix &M)
{
    os << "Matrix {";
    for (int r = 0; r < M.GetNumRows(); ++r)
    {   
        if (r != 0)
        {
            os << "        ";
        }

        for (int c = 0; c < M.GetNumCols(); ++c)
        {
            os << '\t' << M.At(r, c);
        }

        if (r == M.GetNumCols() - 1)
        {
            os << "\t}";
        }

        os << '\n';
    }
    return os;
}

Matrix operator+(const Matrix &LHS, const Matrix &RHS)
{
    if (LHS.GetNumRows() != RHS.GetNumRows() || LHS.GetNumCols() != RHS.GetNumCols())
        throw std::invalid_argument("Dimension of the two matrices must match.");

    Matrix Res(LHS.GetNumRows(), LHS.GetNumCols());
    for (int r = 0; r < LHS.GetNumRows(); ++r)
    {
        for (int c = 0; c < LHS.GetNumCols(); ++c)
        {
            Res(r, c) = LHS.At(r, c) + RHS.At(r, c);
        }
    }
    return Res;
}

Matrix operator-(const Matrix &LHS, const Matrix &RHS)
{
    if (LHS.GetNumRows() != RHS.GetNumRows() || LHS.GetNumCols() != RHS.GetNumCols())
        throw std::invalid_argument("Dimension of the two matrices must match.");

    Matrix Res(LHS.GetNumRows(), LHS.GetNumCols());
    for (int r = 0; r < LHS.GetNumRows(); ++r)
    {
        for (int c = 0; c < LHS.GetNumCols(); ++c)
        {
            Res(r, c) = LHS.At(r, c) - RHS.At(r, c);
        }
    }
    return Res;
}

Matrix operator*(const Matrix &LHS, const Matrix &RHS)
{
    if (LHS.GetNumRows() != RHS.GetNumRows() || LHS.GetNumCols() != RHS.GetNumCols())
        throw std::invalid_argument("Dimension of the two matrices must match.");

    Matrix Res(LHS.GetNumRows(), LHS.GetNumCols());
    for (int r = 0; r < LHS.GetNumRows(); ++r)
    {
        for (int c = 0; c < LHS.GetNumCols(); ++c)
        {
            Res(r, c) = LHS.At(r, c) * RHS.At(r, c);
        }
    }
    return Res;
}

Matrix operator/(const Matrix &LHS, const Matrix &RHS)
{
    if (LHS.GetNumRows() != RHS.GetNumRows() || LHS.GetNumCols() != RHS.GetNumCols())
        throw std::invalid_argument("Dimension of the two matrices must match.");

    Matrix Res(LHS.GetNumRows(), LHS.GetNumCols());
    for (int r = 0; r < LHS.GetNumRows(); ++r)
    {
        for (int c = 0; c < LHS.GetNumCols(); ++c)
        {
            Res(r, c) = LHS.At(r, c) / RHS.At(r, c);
        }
    }
    return Res;
}

Matrix Matrix::Mul(const Matrix &RHS)
{
    if (this->GetNumCols() != RHS.GetNumRows())
        throw std::invalid_argument("LHS matrix's rows must match RHS matrix's columns.");

    int Dim = this->GetNumCols();

    Matrix Res(this->GetNumRows(), RHS.GetNumCols());
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

Matrix Matrix::IdentityMatrix(int Size)
{
    Matrix I(Size, Size);
    for (int i = 0; i < Size; ++i)
    {
        I(i, i) = 1.f;
    }
    return I;
}

TEST_CASE("constructing and inspecting a 4x4 matrix")
{
    Matrix M(4, 4);
    M(0, 0) = 1.f;
    M(0, 1) = 2.f;
    M(0, 2) = 3.f;
    M(0, 3) = 4.f;
    M(1, 0) = 5.5f;
    M(1, 1) = 6.5f;
    M(1, 2) = 7.5f;
    M(1, 3) = 8.5f;
    M(2, 0) = 9.f;
    M(2, 1) = 10.f;
    M(2, 2) = 11.f;
    M(2, 3) = 12.f;
    M(3, 0) = 13.5f;
    M(3, 1) = 14.5f;
    M(3, 2) = 15.5f;
    M(3, 3) = 16.5f;

    CHECK(M(0, 0) == 1.f);
    CHECK(M(0, 3) == 4.f);
    CHECK(M(1, 0) == 5.5f);
    CHECK(M(1, 2) == 7.5f);
    CHECK(M(2, 2) == 11.f);
    CHECK(M(3, 0) == 13.5f);
    CHECK(M(3, 2) == 15.5f);
}

TEST_CASE("compare two matrices")
{
    Matrix M(4, 4);
    M(0, 0) = 1.f;
    M(0, 1) = 2.f;
    M(0, 2) = 3.f;
    M(0, 3) = 4.f;
    M(1, 0) = 5.5f;
    M(1, 1) = 6.5f;
    M(1, 2) = 7.5f;
    M(1, 3) = 8.5f;
    M(2, 0) = 9.f;
    M(2, 1) = 10.f;
    M(2, 2) = 11.f;
    M(2, 3) = 12.f;
    M(3, 0) = 13.5f;
    M(3, 1) = 14.5f;
    M(3, 2) = 15.5f;
    M(3, 3) = 16.5f;

    Matrix N(4, 4);
    N(0, 0) = 1.f;
    N(0, 1) = 2.f;
    N(0, 2) = 3.f;
    N(0, 3) = 4.f;
    N(1, 0) = 5.5f;
    N(1, 1) = 6.5f;
    N(1, 2) = 7.5f;
    N(1, 3) = 8.5f;
    N(2, 0) = 9.f;
    N(2, 1) = 10.f;
    N(2, 2) = 11.f;
    N(2, 3) = 12.f;
    N(3, 0) = 13.5f;
    N(3, 1) = 14.5f;
    N(3, 2) = 15.5f;
    N(3, 3) = 16.5f;

    CHECK(M == N);

    Matrix A(4, 4, 1.f);
    Matrix B(4, 4, 1.f);
    CHECK(M != A);
    CHECK(A == B);
}

TEST_CASE("2x2 matrix")
{
    Matrix A(2, 2);
    A(0, 0) = -3.f;
    A(0, 1) = 5.f;
    A(1, 0) = 1.f;
    A(1, 1) = -2.f;

    CHECK(A.At(0, 0) == -3.f);
    CHECK(A.At(0, 1) == 5.f);
    CHECK(A.At(1, 0) == 1.f);
    CHECK(A.At(1, 1) == -2.f);
}

TEST_CASE("3x3 matrix")
{
    Matrix A(3, 3);
    A(0, 0) = -3.f;
    A(0, 1) = 5.f;
    A(0, 2) = 0.f;
    A(1, 0) = 1.f;
    A(1, 1) = -2.f;
    A(1, 2) = -7.f;
    A(2, 0) = 0.f;
    A(2, 1) = 1.f;
    A(2, 2) = 1.f;

    CHECK(A.At(0, 0) == -3.f);
    CHECK(A.At(1, 1) == -2.f);
    CHECK(A.At(2, 2) == 1.f);
}

TEST_CASE("matrix operators (+, -, *, /)")
{
    Matrix A(2, 2);
    A(0, 0) = -3.f;
    A(0, 1) = 5.f;
    A(1, 0) = 1.f;
    A(1, 1) = -2.f;

    Matrix B(2, 2, 1.f);

    Matrix C(2, 2);
    C(0, 0) = -2.f;
    C(0, 1) = 6.f;
    C(1, 0) = 2.f;
    C(1, 1) = -1.f;

    Matrix D(2, 2);
    D(0, 0) = -4.f;
    D(0, 1) = 4.f;
    D(1, 0) = 0.f;
    D(1, 1) = -3.f;

    CHECK(A + B == C);
    CHECK(A * B == A);
    CHECK(A - B == D);
}

TEST_CASE("multiplying two matrices")
{
    Matrix M(4, 4);
    M(0, 0) = 1.f;
    M(0, 1) = 2.f;
    M(0, 2) = 3.f;
    M(0, 3) = 4.f;
    M(1, 0) = 5.f;
    M(1, 1) = 6.f;
    M(1, 2) = 7.f;
    M(1, 3) = 8.f;
    M(2, 0) = 9.f;
    M(2, 1) = 8.f;
    M(2, 2) = 7.f;
    M(2, 3) = 6.f;
    M(3, 0) = 5.f;
    M(3, 1) = 4.f;
    M(3, 2) = 3.f;
    M(3, 3) = 2.f;

    Matrix N(4, 4);
    N(0, 0) = -2.f;
    N(0, 1) = 1.f;
    N(0, 2) = 2.f;
    N(0, 3) = 3.f;
    N(1, 0) = 3.f;
    N(1, 1) = 2.f;
    N(1, 2) = 1.f;
    N(1, 3) = -1.f;
    N(2, 0) = 4.f;
    N(2, 1) = 3.f;
    N(2, 2) = 6.f;
    N(2, 3) = 5.f;
    N(3, 0) = 1.f;
    N(3, 1) = 2.f;
    N(3, 2) = 7.f;
    N(3, 3) = 8.f;

    Matrix A(4, 4);
    A(0, 0) = 20.f;
    A(0, 1) = 22.f;
    A(0, 2) = 50.f;
    A(0, 3) = 48.f;
    A(1, 0) = 44.f;
    A(1, 1) = 54.f;
    A(1, 2) = 114.f;
    A(1, 3) = 108.f;
    A(2, 0) = 40.f;
    A(2, 1) = 58.f;
    A(2, 2) = 110.f;
    A(2, 3) = 102.f;
    A(3, 0) = 16.f;
    A(3, 1) = 26.f;
    A(3, 2) = 46.f;
    A(3, 3) = 42.f;

    CHECK(M.Mul(N) == A);
}

TEST_CASE("a matrix multiplied by a tuple")
{
    Matrix M(4, 4);
    M(0, 0) = 1.f;
    M(0, 1) = 2.f;
    M(0, 2) = 3.f;
    M(0, 3) = 4.f;
    M(1, 0) = 2.f;
    M(1, 1) = 4.f;
    M(1, 2) = 4.f;
    M(1, 3) = 2.f;
    M(2, 0) = 8.f;
    M(2, 1) = 6.f;
    M(2, 2) = 4.f;
    M(2, 3) = 1.f;
    M(3, 0) = 0.f;
    M(3, 1) = 0.f;
    M(3, 2) = 0.f;
    M(3, 3) = 1.f;

    Point N(1.f, 2.f, 3.f);

    Point A(18.f, 24.f, 33.f);

    CHECK(M.Mul(N) == A);
}

TEST_CASE("test identity matrix")
{
    Matrix M(3, 3);
    M(0, 0) = 1.f;
    M(1, 1) = 1.f;
    M(2, 2) = 1.f;

    CHECK(M == Matrix::IdentityMatrix(3));
}

TEST_CASE("multiplying a matrix by the identity matrix")
{
    Matrix M(4, 4);
    M(0, 0) = 0.f;
    M(0, 1) = 1.f;
    M(0, 2) = 2.f;
    M(0, 3) = 4.f;
    M(1, 0) = 1.f;
    M(1, 1) = 2.f;
    M(1, 2) = 4.f;
    M(1, 3) = 8.f;
    M(2, 0) = 2.f;
    M(2, 1) = 4.f;
    M(2, 2) = 8.f;
    M(2, 3) = 16.f;
    M(3, 0) = 4.f;
    M(3, 1) = 8.f;
    M(3, 2) = 16.f;
    M(3, 3) = 32.f;

    CHECK(M.Mul(Matrix::IdentityMatrix(4)) == M);
}

TEST_CASE("multiplying the identity matrix by a tuple")
{
    Tuple T(1.f, 2.f, 3.f, 4.f);

    CHECK(Matrix::IdentityMatrix(4).Mul(T) == T);
}