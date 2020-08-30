#include <iostream>
#include <exception>
#include <stdexcept>
#include <vector>
#include <string>
#include <cmath>
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

Matrix Matrix::Identity(int Size)
{
    Matrix I(Size, Size);
    for (int i = 0; i < Size; ++i)
    {
        I(i, i) = 1.f;
    }
    return I;
}

Matrix Matrix::T()
{
    Matrix Res(numCols, numRows);
    for (int r = 0; r < numRows; ++r)
    {
        for (int c = 0; c < numCols; ++c)
        {
            Res(c, r) = this->At(r, c);
        }
    }

    return Res;
}

float Matrix::Determinant()
{
    if (numRows != numCols)
        throw std::invalid_argument("only square matrix has determinant");

    if (numRows == 1)
        return this->At(0, 0);
    else if (numRows == 2)
    {
        return this->At(0, 0) * this->At(1, 1) - this->At(0, 1) * this->At(1, 0);
    }
    else
    {
        float Det = 0.f;
        for (int c = 0; c < numCols; ++c)
        {
            Det += this->At(0, c) * Cofactor(0, c);
        }
        return Det;
    }
}

Matrix Matrix::Submatrix(int RowRemoved, int ColRemoved)
{
    if (!IsValid(RowRemoved, ColRemoved))
        throw std::invalid_argument("Invalid Row or Col value");

    Matrix Res(numRows - 1, numCols - 1);

    int CurRow = 0;
    int CurCol = 0;

    for (int r = 0; r < numRows; ++r)
    {
        if (r == RowRemoved) continue;
        CurCol = 0;
        for (int c = 0; c < numCols; ++c)
        {
            if (c == ColRemoved) continue;
            Res(CurRow, CurCol) = this->At(r, c);
            ++CurCol;
        }
        ++CurRow;
    }

    return Res;
}

float Matrix::Minor(int Row, int Col)
{
    return Submatrix(Row, Col).Determinant();
}

float Matrix::Cofactor(int Row, int Col)
{
    float Minor = Submatrix(Row, Col).Determinant();
    if ((Row + Col) % 2 == 1) return -Minor;
    return Minor;
}

bool Matrix::IsInvertible()
{
    return Determinant() != 0;
}

Matrix Matrix::Inverse()
{
    float Det = Determinant();
    if (Util::Equal(Det, 0.f))
        throw std::invalid_argument("this matrix is not invertible");

    Matrix Res(numRows, numCols);

    for (int r = 0; r < numRows; ++r)
    {
        for (int c = 0; c < numCols; ++c)
        {
            Res(c, r) = Cofactor(r, c) / Det;
        }
    }

    return Res;
}

Matrix Matrix::Translation(float X, float Y, float Z)
{
    Matrix Res = Identity(4);
    Res(0, 3) = X;
    Res(1, 3) = Y;
    Res(2, 3) = Z;
    return Res;
}

Matrix Matrix::Scaling(float X, float Y, float Z)
{
    Matrix Res = Identity(4);
    Res(0, 0) = X;
    Res(1, 1) = Y;
    Res(2, 2) = Z;
    return Res;
}

Matrix Matrix::RotationX(float Rad)
{
    Matrix Res = Identity(4);
    float Sin = std::sin(Rad);
    float Cos = std::cos(Rad);
    Res(1, 1) = Cos;
    Res(1, 2) = -Sin;
    Res(2, 1) = Sin;
    Res(2, 2) = Cos;
    return Res;
}

Matrix Matrix::RotationY(float Rad)
{
    Matrix Res = Identity(4);
    float Sin = std::sin(Rad);
    float Cos = std::cos(Rad);
    Res(0, 0) = Cos;
    Res(0, 2) = Sin;
    Res(2, 0) = -Sin;
    Res(2, 2) = Cos;
    return Res;
}

Matrix Matrix::RotationZ(float Rad)
{
    Matrix Res = Identity(4);
    float Sin = std::sin(Rad);
    float Cos = std::cos(Rad);
    Res(0, 0) = Cos;
    Res(0, 1) = -Sin;
    Res(1, 0) = Sin;
    Res(1, 1) = Cos;
    return Res;
}

Matrix Matrix::Shearing(float XY, float XZ, float YX, float YZ, float ZX, float ZY)
{
    Matrix Res = Identity(4);
    Res(0, 1) = XY;
    Res(0, 2) = XZ;
    Res(1, 0) = YX;
    Res(1, 2) = YZ;
    Res(2, 0) = ZX;
    Res(2, 1) = ZY;
    return Res;
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

    CHECK(M == Matrix::Identity(3));
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

    CHECK(M.Mul(Matrix::Identity(4)) == M);
}

TEST_CASE("multiplying the identity matrix by a tuple")
{
    Tuple T(1.f, 2.f, 3.f, 4.f);

    CHECK(Matrix::Identity(4).Mul(T) == T);
}

TEST_CASE("transposing a matrix")
{
    Matrix M(4, 4);
    M(0, 0) = 0.f;
    M(0, 1) = 9.f;
    M(0, 2) = 3.f;
    M(0, 3) = 0.f;
    M(1, 0) = 9.f;
    M(1, 1) = 8.f;
    M(1, 2) = 0.f;
    M(1, 3) = 8.f;
    M(2, 0) = 1.f;
    M(2, 1) = 8.f;
    M(2, 2) = 5.f;
    M(2, 3) = 3.f;
    M(3, 0) = 0.f;
    M(3, 1) = 0.f;
    M(3, 2) = 5.f;
    M(3, 3) = 8.f;

    Matrix T(4, 4);
    T(0, 0) = 0.f;
    T(0, 1) = 9.f;
    T(0, 2) = 1.f;
    T(0, 3) = 0.f;
    T(1, 0) = 9.f;
    T(1, 1) = 8.f;
    T(1, 2) = 8.f;
    T(1, 3) = 0.f;
    T(2, 0) = 3.f;
    T(2, 1) = 0.f;
    T(2, 2) = 5.f;
    T(2, 3) = 5.f;
    T(3, 0) = 0.f;
    T(3, 1) = 8.f;
    T(3, 2) = 3.f;
    T(3, 3) = 8.f;

    CHECK(M.T() == T);
}

TEST_CASE("calculating the determinant of a 2x2 matrix")
{
    Matrix A(2, 2);
    A(0, 0) = 1.f;
    A(0, 1) = 5.f;
    A(1, 0) = -3.f;
    A(1, 1) = 2.f;

    CHECK(Util::Equal(A.Determinant(), 17.f));
}

TEST_CASE("a submatrix of a 3x3 matrix is a 2x2 matrix")
{
    Matrix A(3, 3);
    A(0, 0) = 1.f;
    A(0, 1) = 5.f;
    A(0, 2) = 0.f;
    A(1, 0) = -3.f;
    A(1, 1) = 2.f;
    A(1, 2) = 7.f;
    A(2, 0) = 0.f;
    A(2, 1) = 6.f;
    A(2, 2) = -3.f;

    Matrix B(2, 2);
    B(0, 0) = -3.f;
    B(0, 1) = 2.f;
    B(1, 0) = 0.f;
    B(1, 1) = 6.f;

    CHECK(A.Submatrix(0, 2) == B);
}

TEST_CASE("a submatrix of a 4x4 matrix is a 3x3 matrix")
{
    Matrix A(4, 4);
    A(0, 0) = -6.f;
    A(0, 1) = 1.f;
    A(0, 2) = 1.f;
    A(0, 3) = 6.f;
    A(1, 0) = -8.f;
    A(1, 1) = 5.f;
    A(1, 2) = 8.f;
    A(1, 3) = 6.f;
    A(2, 0) = -1.f;
    A(2, 1) = 0.f;
    A(2, 2) = 8.f;
    A(2, 3) = 2.f;
    A(3, 0) = -7.f;
    A(3, 1) = 1.f;
    A(3, 2) = -1.f;
    A(3, 3) = 1.f;

    Matrix B(3, 3);
    B(0, 0) = -6.f;
    B(0, 1) = 1.f;
    B(0, 2) = 6.f;
    B(1, 0) = -8.f;
    B(1, 1) = 8.f;
    B(1, 2) = 6.f;
    B(2, 0) = -7.f;
    B(2, 1) = -1.f;
    B(2, 2) = 1.f;

    CHECK(A.Submatrix(2, 1) == B);
}

TEST_CASE("calculating a minor of a 3x3 matrix")
{
    Matrix A(3, 3);
    A(0, 0) = 3.f;
    A(0, 1) = 5.f;
    A(0, 2) = 0.f;
    A(1, 0) = 2.f;
    A(1, 1) = -1.f;
    A(1, 2) = -7.f;
    A(2, 0) = 6.f;
    A(2, 1) = -1.f;
    A(2, 2) = 5.f;

    CHECK(Util::Equal(A.Minor(1, 0), 25.f));
}

TEST_CASE("calculating a cofactor of a 3x3 matrix")
{
    Matrix A(3, 3);
    A(0, 0) = 3.f;
    A(0, 1) = 5.f;
    A(0, 2) = 0.f;
    A(1, 0) = 2.f;
    A(1, 1) = -1.f;
    A(1, 2) = -7.f;
    A(2, 0) = 6.f;
    A(2, 1) = -1.f;
    A(2, 2) = 5.f;

    CHECK(Util::Equal(A.Cofactor(1, 0), -25.f));
    CHECK(Util::Equal(A.Cofactor(0, 0), -12.f));
}

TEST_CASE("calculating the determinant of a 3x3 matrix")
{
    Matrix A(3, 3);
    A(0, 0) = 1.f;
    A(0, 1) = 2.f;
    A(0, 2) = 6.f;
    A(1, 0) = -5.f;
    A(1, 1) = 8.f;
    A(1, 2) = -4.f;
    A(2, 0) = 2.f;
    A(2, 1) = 6.f;
    A(2, 2) = 4.f;

    CHECK(Util::Equal(A.Cofactor(0, 0), 56.f));
    CHECK(Util::Equal(A.Cofactor(0, 1), 12.f));
    CHECK(Util::Equal(A.Cofactor(0, 2), -46.f));
 
    CHECK(Util::Equal(A.Determinant(), -196.f));
}

TEST_CASE("calculating the determinant of a 4x4 matrix")
{
    Matrix A(4, 4);
    A(0, 0) = -2.f;
    A(0, 1) = -8.f;
    A(0, 2) = 3.f;
    A(0, 3) = 5.f;
    A(1, 0) = -3.f;
    A(1, 1) = 1.f;
    A(1, 2) = 7.f;
    A(1, 3) = 3.f;
    A(2, 0) = 1.f;
    A(2, 1) = 2.f;
    A(2, 2) = -9.f;
    A(2, 3) = 6.f;
    A(3, 0) = -6.f;
    A(3, 1) = 7.f;
    A(3, 2) = 7.f;
    A(3, 3) = -9.f;

    CHECK(Util::Equal(A.Cofactor(0, 0), 690.f));
    CHECK(Util::Equal(A.Cofactor(0, 1), 447.f));
    CHECK(Util::Equal(A.Cofactor(0, 2), 210.f));
    CHECK(Util::Equal(A.Cofactor(0, 3), 51.f));
    CHECK(Util::Equal(A.Determinant(), -4071.f));
}

TEST_CASE("testing the invertible matrix for invertibility")
{
    Matrix A(4, 4);
    A(0, 0) = 6.f;
    A(0, 1) = 4.f;
    A(0, 2) = 4.f;
    A(0, 3) = 4.f;
    A(1, 0) = 5.f;
    A(1, 1) = 5.f;
    A(1, 2) = 7.f;
    A(1, 3) = 6.f;
    A(2, 0) = 4.f;
    A(2, 1) = -9.f;
    A(2, 2) = 3.f;
    A(2, 3) = -7.f;
    A(3, 0) = 9.f;
    A(3, 1) = 1.f;
    A(3, 2) = 7.f;
    A(3, 3) = -6.f;

    CHECK(Util::Equal(A.Determinant(), -2120.f));
    CHECK(A.IsInvertible());
}

TEST_CASE("testing the noninvertible matrix for invertibility")
{
    Matrix A(4, 4);
    A(0, 0) = -4.f;
    A(0, 1) = 2.f;
    A(0, 2) = -2.f;
    A(0, 3) = -3.f;
    A(1, 0) = 9.f;
    A(1, 1) = 6.f;
    A(1, 2) = 2.f;
    A(1, 3) = 6.f;
    A(2, 0) = 0.f;
    A(2, 1) = -5.f;
    A(2, 2) = 1.f;
    A(2, 3) = -5.f;
    A(3, 0) = 0.f;
    A(3, 1) = 0.f;
    A(3, 2) = 0.f;
    A(3, 3) = 0.f;

    CHECK(Util::Equal(A.Determinant(), 0.f));
    CHECK(!A.IsInvertible());
}

TEST_CASE("calculating the inverse of a matrix")
{
    Matrix A(4, 4);
    A(0, 0) = -5.f;
    A(0, 1) = 2.f;
    A(0, 2) = 6.f;
    A(0, 3) = -8.f;
    A(1, 0) = 1.f;
    A(1, 1) = -5.f;
    A(1, 2) = 1.f;
    A(1, 3) = 8.f;
    A(2, 0) = 7.f;
    A(2, 1) = 7.f;
    A(2, 2) = -6.f;
    A(2, 3) = -7.f;
    A(3, 0) = 1.f;
    A(3, 1) = -3.f;
    A(3, 2) = 7.f;
    A(3, 3) = 4.f;

    CHECK(Util::Equal(A.Determinant(), 532.f));
    CHECK(Util::Equal(A.Cofactor(2, 3), -160.f));
    CHECK(Util::Equal(A.Cofactor(3, 2), 105.f));

    CHECK(A.Cofactor(2, 3) == -160.f);
    CHECK(A.Cofactor(3, 2) == 105.f);

    Matrix B(4, 4);
    B(0, 0) = 0.21805f;
    B(0, 1) = 0.45113f;
    B(0, 2) = 0.24060f;
    B(0, 3) = -0.04511f;
    B(1, 0) = -0.80827f;
    B(1, 1) = -1.45677f;
    B(1, 2) = -0.44361f;
    B(1, 3) = 0.52068f;
    B(2, 0) = -0.07895f;
    B(2, 1) = -0.22368f;
    B(2, 2) = -0.05263f;
    B(2, 3) = 0.19737f;
    B(3, 0) = -0.52256f;
    B(3, 1) = -0.81391f;
    B(3, 2) = -0.30075f;
    B(3, 3) = 0.30639f;

    CHECK(A.Inverse() == B);
}

TEST_CASE("calculating the inverse of another matrix")
{
    Matrix A(4, 4);
    A(0, 0) = 8.f;
    A(0, 1) = -5.f;
    A(0, 2) = 9.f;
    A(0, 3) = 2.f;
    A(1, 0) = 7.f;
    A(1, 1) = 5.f;
    A(1, 2) = 6.f;
    A(1, 3) = 1.f;
    A(2, 0) = -6.f;
    A(2, 1) = 0.f;
    A(2, 2) = 9.f;
    A(2, 3) = 6.f;
    A(3, 0) = -3.f;
    A(3, 1) = 0.f;
    A(3, 2) = -9.f;
    A(3, 3) = -4.f;

    Matrix B(4, 4);
    B(0, 0) = -0.15385f;
    B(0, 1) = -0.15385f;
    B(0, 2) = -0.28205f;
    B(0, 3) = -0.53846f;

    B(1, 0) = -0.07692f;
    B(1, 1) = 0.12308f;
    B(1, 2) = 0.02564f;
    B(1, 3) = 0.03077f;

    B(2, 0) = 0.35897f;
    B(2, 1) = 0.35897f;
    B(2, 2) = 0.43590f;
    B(2, 3) = 0.92308f;

    B(3, 0) = -0.69231f;
    B(3, 1) = -0.69231f;
    B(3, 2) = -0.76923f;
    B(3, 3) = -1.92308f;

    CHECK(A.Inverse() == B);
}

TEST_CASE("calculating the inverse of a third matrix")
{
    Matrix A(4, 4);
    A(0, 0) = 9.f;
    A(0, 1) = 3.f;
    A(0, 2) = 0.f;
    A(0, 3) = 9.f;
    A(1, 0) = -5.f;
    A(1, 1) = -2.f;
    A(1, 2) = -6.f;
    A(1, 3) = -3.f;
    A(2, 0) = -4.f;
    A(2, 1) = 9.f;
    A(2, 2) = 6.f;
    A(2, 3) = 4.f;
    A(3, 0) = -7.f;
    A(3, 1) = 6.f;
    A(3, 2) = 6.f;
    A(3, 3) = 2.f;

    Matrix B(4, 4);
    B(0, 0) = -0.04074f;
    B(0, 1) = -0.07778f;
    B(0, 2) = 0.14444f;
    B(0, 3) = -0.22222f;

    B(1, 0) = -0.07778f;
    B(1, 1) = 0.03333f;
    B(1, 2) = 0.36667f;
    B(1, 3) = -0.33333f;

    B(2, 0) = -0.02901f;
    B(2, 1) = -0.14630f;
    B(2, 2) = -0.10926f;
    B(2, 3) = 0.12963f;

    B(3, 0) = 0.17778f;
    B(3, 1) = 0.06667f;
    B(3, 2) = -0.26667f;
    B(3, 3) = 0.33333f;

    CHECK(A.Inverse() == B);
}

TEST_CASE("multiplying a product by its inverse")
{
    Matrix A(4, 4);
    A(0, 0) = 3.f;
    A(0, 1) = -9.f;
    A(0, 2) = 7.f;
    A(0, 3) = 3.f;
    A(1, 0) = 3.f;
    A(1, 1) = -8.f;
    A(1, 2) = 2.f;
    A(1, 3) = -9.f;
    A(2, 0) = -4.f;
    A(2, 1) = 4.f;
    A(2, 2) = 4.f;
    A(2, 3) = 1.f;
    A(3, 0) = -6.f;
    A(3, 1) = 5.f;
    A(3, 2) = -1.f;
    A(3, 3) = 1.f;

    Matrix B(4, 4);
    B(0, 0) = 8.f;
    B(0, 1) = 2.f;
    B(0, 2) = 2.f;
    B(0, 3) = 2.f;
    B(1, 0) = 3.f;
    B(1, 1) = -1.f;
    B(1, 2) = 7.f;
    B(1, 3) = 0.f;
    B(2, 0) = 7.f;
    B(2, 1) = 0.f;
    B(2, 2) = 5.f;
    B(2, 3) = 4.f;
    B(3, 0) = 6.f;
    B(3, 1) = -2.f;
    B(3, 2) = 0.f;
    B(3, 3) = 5.f;

    Matrix C = A.Mul(B);
    CHECK(A == C.Mul(B.Inverse()));
}

TEST_CASE("Test matrix properties")
{
    Matrix I = Matrix::Identity(4);
    CHECK(I.Inverse() == I);

    Matrix A(4, 4);
    A(0, 0) = 3.f;
    A(0, 1) = -9.f;
    A(0, 2) = 7.f;
    A(0, 3) = 3.f;
    A(1, 0) = 3.f;
    A(1, 1) = -8.f;
    A(1, 2) = 2.f;
    A(1, 3) = -9.f;
    A(2, 0) = -4.f;
    A(2, 1) = 4.f;
    A(2, 2) = 4.f;
    A(2, 3) = 1.f;
    A(3, 0) = -6.f;
    A(3, 1) = 5.f;
    A(3, 2) = -1.f;
    A(3, 3) = 1.f;

    CHECK(A.Mul(A.Inverse()) == I);

    CHECK(A.Inverse().T() == A.T().Inverse());

    Matrix I2 = Matrix::Identity(4);
    I2(0, 1) = 2.f;
    Tuple B = Tuple(4.f, 5.f, 6.f, 7.f);

    CHECK(I2.Mul(B) != B);
}

TEST_CASE("Multiplying by a translation matrix")
{
    Matrix T = Matrix::Translation(5.f, -3.f, 2.f);
    Point P(-3.f, 4.f, 5.f);

    CHECK(T.Mul(P) == Point(2.f, 1.f, 7.f));
}

TEST_CASE("Multiplying by the inverse of a translation matrix")
{
    Matrix T = Matrix::Translation(5.f, -3.f, 2.f);
    auto Inv = T.Inverse();
    Point P(-3.f, 4.f, 5.f);

    CHECK(Inv.Mul(P) == Point(-8.f, 7.f, 3.f));
}

TEST_CASE("Translation does not affect vectors")
{
    Matrix T = Matrix::Translation(5.f, -3.f, 2.f);
    Vector V(-3.f, 4.f, 5.f);

    CHECK(T.Mul(V) == V);
}

TEST_CASE("A scaling matrix applied to a point")
{
    Matrix S = Matrix::Scaling(2.f, 3.f, 4.f);
    Point P(-4.f, 6.f, 8.f);

    CHECK(S.Mul(P) == Point(-8.f, 18.f, 32.f));
}

TEST_CASE("A scaling matrix applied to a vector")
{
    Matrix S = Matrix::Scaling(2.f, 3.f, 4.f);
    Vector V(-4.f, 6.f, 8.f);

    CHECK(S.Mul(V) == Vector(-8.f, 18.f, 32.f));
}

TEST_CASE("Multiplying by the inverse of a scaling matrix")
{
    Matrix S = Matrix::Scaling(2.f, 3.f, 4.f);
    Vector V(-4.f, 6.f, 8.f);

    CHECK(S.Inverse().Mul(V) == Vector(-2.f, 2.f, 2.f));
}

TEST_CASE("Reflection is scaling by a negative value")
{
    Matrix S = Matrix::Scaling(-1.f, 1.f, 1.f);
    Point P(2.f, 3.f, 4.f);

    CHECK(S.Mul(P) == Point(-2.f, 3.f, 4.f));
}

TEST_CASE("Rotating a point around the x axis")
{
    Point P(0.f, 1.f, 0.f);
    Matrix R4 = Matrix::RotationX(M_PI/4);
    Matrix R2 = Matrix::RotationX(M_PI/2);

    CHECK(R4.Mul(P) == Point(0.f, std::sqrt(2.f)/2, std::sqrt(2.f)/2));
    CHECK(R2.Mul(P) == Point(0.f, 0.f, 1.f));
}

TEST_CASE("The inverse of an x-rotation rotate in the opposite direction")
{
    Point P(0.f, 1.f, 0.f);
    Matrix R4 = Matrix::RotationX(M_PI/4);

    CHECK(R4.Inverse().Mul(P) == Point(0.f, std::sqrt(2.f)/2, -std::sqrt(2.f)/2));
}

TEST_CASE("Rotating a point around the x axis")
{
    Point P(0.f, 1.f, 0.f);
    Matrix R4 = Matrix::RotationX(M_PI/4);
    Matrix R2 = Matrix::RotationX(M_PI/2);

    CHECK(R4.Mul(P) == Point(0.f, std::sqrt(2.f)/2, std::sqrt(2.f)/2));
    CHECK(R2.Mul(P) == Point(0.f, 0.f, 1.f));
}

TEST_CASE("Rotating a point around the y axis")
{
    Point P(0.f, 0.f, 1.f);
    Matrix R4 = Matrix::RotationY(M_PI/4);
    Matrix R2 = Matrix::RotationY(M_PI/2);

    CHECK(R4.Mul(P) == Point(std::sqrt(2.f)/2, 0.f, std::sqrt(2.f)/2));
    CHECK(R2.Mul(P) == Point(1.f, 0.f, 0.f));
}

TEST_CASE("Rotating a point around the z axis")
{
    Point P(0.f, 1.f, 0.f);
    Matrix R4 = Matrix::RotationZ(M_PI/4);
    Matrix R2 = Matrix::RotationZ(M_PI/2);

    CHECK(R4.Mul(P) == Point(-std::sqrt(2.f)/2, std::sqrt(2.f)/2, 0.f));
    CHECK(R2.Mul(P) == Point(-1.f, 0.f, 0.f));
}

TEST_CASE("A shearing transformation moves x in proportion to y")
{
    Point P(2.f, 3.f, 4.f);
    Matrix S = Matrix::Shearing(1.f, 0.f, 0.f, 0.f, 0.f, 0.f);

    CHECK(S.Mul(P) == Point(5.f, 3.f, 4.f));
}

TEST_CASE("A shearing transformation moves x in proportion to z")
{
    Point P(2.f, 3.f, 4.f);
    Matrix S = Matrix::Shearing(0.f, 1.f, 0.f, 0.f, 0.f, 0.f);

    CHECK(S.Mul(P) == Point(6.f, 3.f, 4.f));
}

TEST_CASE("A shearing transformation moves y in proportion to x")
{
    Point P(2.f, 3.f, 4.f);
    Matrix S = Matrix::Shearing(0.f, 0.f, 1.f, 0.f, 0.f, 0.f);

    CHECK(S.Mul(P) == Point(2.f, 5.f, 4.f));
}

TEST_CASE("A shearing transformation moves y in proportion to z")
{
    Point P(2.f, 3.f, 4.f);
    Matrix S = Matrix::Shearing(0.f, 0.f, 0.f, 1.f, 0.f, 0.f);

    CHECK(S.Mul(P) == Point(2.f, 7.f, 4.f));
}

TEST_CASE("A shearing transformation moves z in proportion to x")
{
    Point P(2.f, 3.f, 4.f);
    Matrix S = Matrix::Shearing(0.f, 0.f, 0.f, 0.f, 1.f, 0.f);

    CHECK(S.Mul(P) == Point(2.f, 3.f, 6.f));
}

TEST_CASE("A shearing transformation moves z in proportion to y")
{
    Point P(2.f, 3.f, 4.f);
    Matrix S = Matrix::Shearing(0.f, 0.f, 0.f, 0.f, 0.f, 1.f);

    CHECK(S.Mul(P) == Point(2.f, 3.f, 7.f));
}