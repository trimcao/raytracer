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

Matrix::Matrix(int NumRows, int NumCols, double Val)
{
    numRows = NumRows;
    numCols = NumCols;
    m = std::vector<std::vector<double>>(numRows, std::vector<double>(numCols, Val));
}

Matrix::Matrix(int NumRows, int NumCols) : Matrix(NumRows, NumCols, 0.)
{
}

Matrix::Matrix(double X, double Y, double Z, double W) : Matrix(4, 1, 0.)
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
        I(i, i) = 1.;
    }
    return I;
}

Matrix Matrix::Identity()
{
    return Identity(4);
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

double Matrix::Determinant()
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
        double Det = 0.;
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

double Matrix::Minor(int Row, int Col)
{
    return Submatrix(Row, Col).Determinant();
}

double Matrix::Cofactor(int Row, int Col)
{
    double Minor = Submatrix(Row, Col).Determinant();
    if ((Row + Col) % 2 == 1) return -Minor;
    return Minor;
}

bool Matrix::IsInvertible()
{
    return Determinant() != 0;
}

Matrix Matrix::Inverse()
{
    double Det = Determinant();
    if (Util::Equal(Det, 0.))
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

Matrix Matrix::Translation(double X, double Y, double Z)
{
    Matrix Res = Identity(4);
    Res(0, 3) = X;
    Res(1, 3) = Y;
    Res(2, 3) = Z;
    return Res;
}

Matrix Matrix::Scaling(double X, double Y, double Z)
{
    Matrix Res = Identity(4);
    Res(0, 0) = X;
    Res(1, 1) = Y;
    Res(2, 2) = Z;
    return Res;
}

Matrix Matrix::RotationX(double Rad)
{
    Matrix Res = Identity(4);
    double Sin = std::sin(Rad);
    double Cos = std::cos(Rad);
    Res(1, 1) = Cos;
    Res(1, 2) = -Sin;
    Res(2, 1) = Sin;
    Res(2, 2) = Cos;
    return Res;
}

Matrix Matrix::RotationY(double Rad)
{
    Matrix Res = Identity(4);
    double Sin = std::sin(Rad);
    double Cos = std::cos(Rad);
    Res(0, 0) = Cos;
    Res(0, 2) = Sin;
    Res(2, 0) = -Sin;
    Res(2, 2) = Cos;
    return Res;
}

Matrix Matrix::RotationZ(double Rad)
{
    Matrix Res = Identity(4);
    double Sin = std::sin(Rad);
    double Cos = std::cos(Rad);
    Res(0, 0) = Cos;
    Res(0, 1) = -Sin;
    Res(1, 0) = Sin;
    Res(1, 1) = Cos;
    return Res;
}

Matrix Matrix::Shearing(double XY, double XZ, double YX, double YZ, double ZX, double ZY)
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

Matrix Matrix::Translate(double X, double Y, double Z)
{
    return Translation(X, Y, Z).Mul(*this);
}

Matrix Matrix::Scale(double X, double Y, double Z)
{
    return Scaling(X, Y, Z).Mul(*this);
}

Matrix Matrix::RotateX(double Rad)
{
    return RotationX(Rad).Mul(*this);
}

Matrix Matrix::RotateY(double Rad)
{
    return RotationY(Rad).Mul(*this);
}
Matrix Matrix::RotateZ(double Rad)
{
    return RotationZ(Rad).Mul(*this);
}

Matrix Matrix::Shear(double XY, double XZ, double YX, double YZ, double ZX, double ZY)
{
    return Shearing(XY, XZ, YX, YZ, ZX, ZY).Mul(*this);
}

TEST_CASE("constructing and inspecting a 4x4 matrix")
{
    Matrix M(4, 4);
    M(0, 0) = 1.;
    M(0, 1) = 2.;
    M(0, 2) = 3.;
    M(0, 3) = 4.;
    M(1, 0) = 5.5;
    M(1, 1) = 6.5;
    M(1, 2) = 7.5;
    M(1, 3) = 8.5;
    M(2, 0) = 9.;
    M(2, 1) = 10.;
    M(2, 2) = 11.;
    M(2, 3) = 12.;
    M(3, 0) = 13.5;
    M(3, 1) = 14.5;
    M(3, 2) = 15.5;
    M(3, 3) = 16.5;

    CHECK(M(0, 0) == 1.);
    CHECK(M(0, 3) == 4.);
    CHECK(M(1, 0) == 5.5);
    CHECK(M(1, 2) == 7.5);
    CHECK(M(2, 2) == 11.);
    CHECK(M(3, 0) == 13.5);
    CHECK(M(3, 2) == 15.5);
}

TEST_CASE("compare two matrices")
{
    Matrix M(4, 4);
    M(0, 0) = 1.;
    M(0, 1) = 2.;
    M(0, 2) = 3.;
    M(0, 3) = 4.;
    M(1, 0) = 5.5;
    M(1, 1) = 6.5;
    M(1, 2) = 7.5;
    M(1, 3) = 8.5;
    M(2, 0) = 9.;
    M(2, 1) = 10.;
    M(2, 2) = 11.;
    M(2, 3) = 12.;
    M(3, 0) = 13.5;
    M(3, 1) = 14.5;
    M(3, 2) = 15.5;
    M(3, 3) = 16.5;

    Matrix N(4, 4);
    N(0, 0) = 1.;
    N(0, 1) = 2.;
    N(0, 2) = 3.;
    N(0, 3) = 4.;
    N(1, 0) = 5.5;
    N(1, 1) = 6.5;
    N(1, 2) = 7.5;
    N(1, 3) = 8.5;
    N(2, 0) = 9.;
    N(2, 1) = 10.;
    N(2, 2) = 11.;
    N(2, 3) = 12.;
    N(3, 0) = 13.5;
    N(3, 1) = 14.5;
    N(3, 2) = 15.5;
    N(3, 3) = 16.5;

    CHECK(M == N);

    Matrix A(4, 4, 1.);
    Matrix B(4, 4, 1.);
    CHECK(M != A);
    CHECK(A == B);
}

TEST_CASE("2x2 matrix")
{
    Matrix A(2, 2);
    A(0, 0) = -3.;
    A(0, 1) = 5.;
    A(1, 0) = 1.;
    A(1, 1) = -2.;

    CHECK(A.At(0, 0) == -3.);
    CHECK(A.At(0, 1) == 5.);
    CHECK(A.At(1, 0) == 1.);
    CHECK(A.At(1, 1) == -2.);
}

TEST_CASE("3x3 matrix")
{
    Matrix A(3, 3);
    A(0, 0) = -3.;
    A(0, 1) = 5.;
    A(0, 2) = 0.;
    A(1, 0) = 1.;
    A(1, 1) = -2.;
    A(1, 2) = -7.;
    A(2, 0) = 0.;
    A(2, 1) = 1.;
    A(2, 2) = 1.;

    CHECK(A.At(0, 0) == -3.);
    CHECK(A.At(1, 1) == -2.);
    CHECK(A.At(2, 2) == 1.);
}

TEST_CASE("matrix operators (+, -, *, /)")
{
    Matrix A(2, 2);
    A(0, 0) = -3.;
    A(0, 1) = 5.;
    A(1, 0) = 1.;
    A(1, 1) = -2.;

    Matrix B(2, 2, 1.);

    Matrix C(2, 2);
    C(0, 0) = -2.;
    C(0, 1) = 6.;
    C(1, 0) = 2.;
    C(1, 1) = -1.;

    Matrix D(2, 2);
    D(0, 0) = -4.;
    D(0, 1) = 4.;
    D(1, 0) = 0.;
    D(1, 1) = -3.;

    CHECK(A + B == C);
    CHECK(A * B == A);
    CHECK(A - B == D);
}

TEST_CASE("multiplying two matrices")
{
    Matrix M(4, 4);
    M(0, 0) = 1.;
    M(0, 1) = 2.;
    M(0, 2) = 3.;
    M(0, 3) = 4.;
    M(1, 0) = 5.;
    M(1, 1) = 6.;
    M(1, 2) = 7.;
    M(1, 3) = 8.;
    M(2, 0) = 9.;
    M(2, 1) = 8.;
    M(2, 2) = 7.;
    M(2, 3) = 6.;
    M(3, 0) = 5.;
    M(3, 1) = 4.;
    M(3, 2) = 3.;
    M(3, 3) = 2.;

    Matrix N(4, 4);
    N(0, 0) = -2.;
    N(0, 1) = 1.;
    N(0, 2) = 2.;
    N(0, 3) = 3.;
    N(1, 0) = 3.;
    N(1, 1) = 2.;
    N(1, 2) = 1.;
    N(1, 3) = -1.;
    N(2, 0) = 4.;
    N(2, 1) = 3.;
    N(2, 2) = 6.;
    N(2, 3) = 5.;
    N(3, 0) = 1.;
    N(3, 1) = 2.;
    N(3, 2) = 7.;
    N(3, 3) = 8.;

    Matrix A(4, 4);
    A(0, 0) = 20.;
    A(0, 1) = 22.;
    A(0, 2) = 50.;
    A(0, 3) = 48.;
    A(1, 0) = 44.;
    A(1, 1) = 54.;
    A(1, 2) = 114.;
    A(1, 3) = 108.;
    A(2, 0) = 40.;
    A(2, 1) = 58.;
    A(2, 2) = 110.;
    A(2, 3) = 102.;
    A(3, 0) = 16.;
    A(3, 1) = 26.;
    A(3, 2) = 46.;
    A(3, 3) = 42.;

    CHECK(M.Mul(N) == A);
}

TEST_CASE("a matrix multiplied by a tuple")
{
    Matrix M(4, 4);
    M(0, 0) = 1.;
    M(0, 1) = 2.;
    M(0, 2) = 3.;
    M(0, 3) = 4.;
    M(1, 0) = 2.;
    M(1, 1) = 4.;
    M(1, 2) = 4.;
    M(1, 3) = 2.;
    M(2, 0) = 8.;
    M(2, 1) = 6.;
    M(2, 2) = 4.;
    M(2, 3) = 1.;
    M(3, 0) = 0.;
    M(3, 1) = 0.;
    M(3, 2) = 0.;
    M(3, 3) = 1.;

    Point N(1., 2., 3.);

    Point A(18., 24., 33.);

    CHECK(M.Mul(N) == A);
}

TEST_CASE("test identity matrix")
{
    Matrix M(3, 3);
    M(0, 0) = 1.;
    M(1, 1) = 1.;
    M(2, 2) = 1.;

    CHECK(M == Matrix::Identity(3));
}

TEST_CASE("multiplying a matrix by the identity matrix")
{
    Matrix M(4, 4);
    M(0, 0) = 0.;
    M(0, 1) = 1.;
    M(0, 2) = 2.;
    M(0, 3) = 4.;
    M(1, 0) = 1.;
    M(1, 1) = 2.;
    M(1, 2) = 4.;
    M(1, 3) = 8.;
    M(2, 0) = 2.;
    M(2, 1) = 4.;
    M(2, 2) = 8.;
    M(2, 3) = 16.;
    M(3, 0) = 4.;
    M(3, 1) = 8.;
    M(3, 2) = 16.;
    M(3, 3) = 32.;

    CHECK(M.Mul(Matrix::Identity(4)) == M);
}

TEST_CASE("multiplying the identity matrix by a tuple")
{
    Tuple T(1., 2., 3., 4.);

    CHECK(Matrix::Identity(4).Mul(T) == T);
}

TEST_CASE("transposing a matrix")
{
    Matrix M(4, 4);
    M(0, 0) = 0.;
    M(0, 1) = 9.;
    M(0, 2) = 3.;
    M(0, 3) = 0.;
    M(1, 0) = 9.;
    M(1, 1) = 8.;
    M(1, 2) = 0.;
    M(1, 3) = 8.;
    M(2, 0) = 1.;
    M(2, 1) = 8.;
    M(2, 2) = 5.;
    M(2, 3) = 3.;
    M(3, 0) = 0.;
    M(3, 1) = 0.;
    M(3, 2) = 5.;
    M(3, 3) = 8.;

    Matrix T(4, 4);
    T(0, 0) = 0.;
    T(0, 1) = 9.;
    T(0, 2) = 1.;
    T(0, 3) = 0.;
    T(1, 0) = 9.;
    T(1, 1) = 8.;
    T(1, 2) = 8.;
    T(1, 3) = 0.;
    T(2, 0) = 3.;
    T(2, 1) = 0.;
    T(2, 2) = 5.;
    T(2, 3) = 5.;
    T(3, 0) = 0.;
    T(3, 1) = 8.;
    T(3, 2) = 3.;
    T(3, 3) = 8.;

    CHECK(M.T() == T);
}

TEST_CASE("calculating the determinant of a 2x2 matrix")
{
    Matrix A(2, 2);
    A(0, 0) = 1.;
    A(0, 1) = 5.;
    A(1, 0) = -3.;
    A(1, 1) = 2.;

    CHECK(Util::Equal(A.Determinant(), 17.));
}

TEST_CASE("a submatrix of a 3x3 matrix is a 2x2 matrix")
{
    Matrix A(3, 3);
    A(0, 0) = 1.;
    A(0, 1) = 5.;
    A(0, 2) = 0.;
    A(1, 0) = -3.;
    A(1, 1) = 2.;
    A(1, 2) = 7.;
    A(2, 0) = 0.;
    A(2, 1) = 6.;
    A(2, 2) = -3.;

    Matrix B(2, 2);
    B(0, 0) = -3.;
    B(0, 1) = 2.;
    B(1, 0) = 0.;
    B(1, 1) = 6.;

    CHECK(A.Submatrix(0, 2) == B);
}

TEST_CASE("a submatrix of a 4x4 matrix is a 3x3 matrix")
{
    Matrix A(4, 4);
    A(0, 0) = -6.;
    A(0, 1) = 1.;
    A(0, 2) = 1.;
    A(0, 3) = 6.;
    A(1, 0) = -8.;
    A(1, 1) = 5.;
    A(1, 2) = 8.;
    A(1, 3) = 6.;
    A(2, 0) = -1.;
    A(2, 1) = 0.;
    A(2, 2) = 8.;
    A(2, 3) = 2.;
    A(3, 0) = -7.;
    A(3, 1) = 1.;
    A(3, 2) = -1.;
    A(3, 3) = 1.;

    Matrix B(3, 3);
    B(0, 0) = -6.;
    B(0, 1) = 1.;
    B(0, 2) = 6.;
    B(1, 0) = -8.;
    B(1, 1) = 8.;
    B(1, 2) = 6.;
    B(2, 0) = -7.;
    B(2, 1) = -1.;
    B(2, 2) = 1.;

    CHECK(A.Submatrix(2, 1) == B);
}

TEST_CASE("calculating a minor of a 3x3 matrix")
{
    Matrix A(3, 3);
    A(0, 0) = 3.;
    A(0, 1) = 5.;
    A(0, 2) = 0.;
    A(1, 0) = 2.;
    A(1, 1) = -1.;
    A(1, 2) = -7.;
    A(2, 0) = 6.;
    A(2, 1) = -1.;
    A(2, 2) = 5.;

    CHECK(Util::Equal(A.Minor(1, 0), 25.));
}

TEST_CASE("calculating a cofactor of a 3x3 matrix")
{
    Matrix A(3, 3);
    A(0, 0) = 3.;
    A(0, 1) = 5.;
    A(0, 2) = 0.;
    A(1, 0) = 2.;
    A(1, 1) = -1.;
    A(1, 2) = -7.;
    A(2, 0) = 6.;
    A(2, 1) = -1.;
    A(2, 2) = 5.;

    CHECK(Util::Equal(A.Cofactor(1, 0), -25.));
    CHECK(Util::Equal(A.Cofactor(0, 0), -12.));
}

TEST_CASE("calculating the determinant of a 3x3 matrix")
{
    Matrix A(3, 3);
    A(0, 0) = 1.;
    A(0, 1) = 2.;
    A(0, 2) = 6.;
    A(1, 0) = -5.;
    A(1, 1) = 8.;
    A(1, 2) = -4.;
    A(2, 0) = 2.;
    A(2, 1) = 6.;
    A(2, 2) = 4.;

    CHECK(Util::Equal(A.Cofactor(0, 0), 56.));
    CHECK(Util::Equal(A.Cofactor(0, 1), 12.));
    CHECK(Util::Equal(A.Cofactor(0, 2), -46.));
 
    CHECK(Util::Equal(A.Determinant(), -196.));
}

TEST_CASE("calculating the determinant of a 4x4 matrix")
{
    Matrix A(4, 4);
    A(0, 0) = -2.;
    A(0, 1) = -8.;
    A(0, 2) = 3.;
    A(0, 3) = 5.;
    A(1, 0) = -3.;
    A(1, 1) = 1.;
    A(1, 2) = 7.;
    A(1, 3) = 3.;
    A(2, 0) = 1.;
    A(2, 1) = 2.;
    A(2, 2) = -9.;
    A(2, 3) = 6.;
    A(3, 0) = -6.;
    A(3, 1) = 7.;
    A(3, 2) = 7.;
    A(3, 3) = -9.;

    CHECK(Util::Equal(A.Cofactor(0, 0), 690.));
    CHECK(Util::Equal(A.Cofactor(0, 1), 447.));
    CHECK(Util::Equal(A.Cofactor(0, 2), 210.));
    CHECK(Util::Equal(A.Cofactor(0, 3), 51.));
    CHECK(Util::Equal(A.Determinant(), -4071.));
}

TEST_CASE("testing the invertible matrix for invertibility")
{
    Matrix A(4, 4);
    A(0, 0) = 6.;
    A(0, 1) = 4.;
    A(0, 2) = 4.;
    A(0, 3) = 4.;
    A(1, 0) = 5.;
    A(1, 1) = 5.;
    A(1, 2) = 7.;
    A(1, 3) = 6.;
    A(2, 0) = 4.;
    A(2, 1) = -9.;
    A(2, 2) = 3.;
    A(2, 3) = -7.;
    A(3, 0) = 9.;
    A(3, 1) = 1.;
    A(3, 2) = 7.;
    A(3, 3) = -6.;

    CHECK(Util::Equal(A.Determinant(), -2120.));
    CHECK(A.IsInvertible());
}

TEST_CASE("testing the noninvertible matrix for invertibility")
{
    Matrix A(4, 4);
    A(0, 0) = -4.;
    A(0, 1) = 2.;
    A(0, 2) = -2.;
    A(0, 3) = -3.;
    A(1, 0) = 9.;
    A(1, 1) = 6.;
    A(1, 2) = 2.;
    A(1, 3) = 6.;
    A(2, 0) = 0.;
    A(2, 1) = -5.;
    A(2, 2) = 1.;
    A(2, 3) = -5.;
    A(3, 0) = 0.;
    A(3, 1) = 0.;
    A(3, 2) = 0.;
    A(3, 3) = 0.;

    CHECK(Util::Equal(A.Determinant(), 0.));
    CHECK(!A.IsInvertible());
}

TEST_CASE("calculating the inverse of a matrix")
{
    Matrix A(4, 4);
    A(0, 0) = -5.;
    A(0, 1) = 2.;
    A(0, 2) = 6.;
    A(0, 3) = -8.;
    A(1, 0) = 1.;
    A(1, 1) = -5.;
    A(1, 2) = 1.;
    A(1, 3) = 8.;
    A(2, 0) = 7.;
    A(2, 1) = 7.;
    A(2, 2) = -6.;
    A(2, 3) = -7.;
    A(3, 0) = 1.;
    A(3, 1) = -3.;
    A(3, 2) = 7.;
    A(3, 3) = 4.;

    CHECK(Util::Equal(A.Determinant(), 532.));
    CHECK(Util::Equal(A.Cofactor(2, 3), -160.));
    CHECK(Util::Equal(A.Cofactor(3, 2), 105.));

    CHECK(A.Cofactor(2, 3) == -160.);
    CHECK(A.Cofactor(3, 2) == 105.);

    Matrix B(4, 4);
    B(0, 0) = 0.21805;
    B(0, 1) = 0.45113;
    B(0, 2) = 0.24060;
    B(0, 3) = -0.04511;
    B(1, 0) = -0.80827;
    B(1, 1) = -1.45677;
    B(1, 2) = -0.44361;
    B(1, 3) = 0.52068;
    B(2, 0) = -0.07895;
    B(2, 1) = -0.22368;
    B(2, 2) = -0.05263;
    B(2, 3) = 0.19737;
    B(3, 0) = -0.52256;
    B(3, 1) = -0.81391;
    B(3, 2) = -0.30075;
    B(3, 3) = 0.30639;

    CHECK(A.Inverse() == B);
}

TEST_CASE("calculating the inverse of another matrix")
{
    Matrix A(4, 4);
    A(0, 0) = 8.;
    A(0, 1) = -5.;
    A(0, 2) = 9.;
    A(0, 3) = 2.;
    A(1, 0) = 7.;
    A(1, 1) = 5.;
    A(1, 2) = 6.;
    A(1, 3) = 1.;
    A(2, 0) = -6.;
    A(2, 1) = 0.;
    A(2, 2) = 9.;
    A(2, 3) = 6.;
    A(3, 0) = -3.;
    A(3, 1) = 0.;
    A(3, 2) = -9.;
    A(3, 3) = -4.;

    Matrix B(4, 4);
    B(0, 0) = -0.15385;
    B(0, 1) = -0.15385;
    B(0, 2) = -0.28205;
    B(0, 3) = -0.53846;

    B(1, 0) = -0.07692;
    B(1, 1) = 0.12308;
    B(1, 2) = 0.02564;
    B(1, 3) = 0.03077;

    B(2, 0) = 0.35897;
    B(2, 1) = 0.35897;
    B(2, 2) = 0.43590;
    B(2, 3) = 0.92308;

    B(3, 0) = -0.69231;
    B(3, 1) = -0.69231;
    B(3, 2) = -0.76923;
    B(3, 3) = -1.92308;

    CHECK(A.Inverse() == B);
}

TEST_CASE("calculating the inverse of a third matrix")
{
    Matrix A(4, 4);
    A(0, 0) = 9.;
    A(0, 1) = 3.;
    A(0, 2) = 0.;
    A(0, 3) = 9.;
    A(1, 0) = -5.;
    A(1, 1) = -2.;
    A(1, 2) = -6.;
    A(1, 3) = -3.;
    A(2, 0) = -4.;
    A(2, 1) = 9.;
    A(2, 2) = 6.;
    A(2, 3) = 4.;
    A(3, 0) = -7.;
    A(3, 1) = 6.;
    A(3, 2) = 6.;
    A(3, 3) = 2.;

    Matrix B(4, 4);
    B(0, 0) = -0.04074;
    B(0, 1) = -0.07778;
    B(0, 2) = 0.14444;
    B(0, 3) = -0.22222;

    B(1, 0) = -0.07778;
    B(1, 1) = 0.03333;
    B(1, 2) = 0.36667;
    B(1, 3) = -0.33333;

    B(2, 0) = -0.02901;
    B(2, 1) = -0.14630;
    B(2, 2) = -0.10926;
    B(2, 3) = 0.12963;

    B(3, 0) = 0.17778;
    B(3, 1) = 0.06667;
    B(3, 2) = -0.26667;
    B(3, 3) = 0.33333;

    CHECK(A.Inverse() == B);
}

TEST_CASE("multiplying a product by its inverse")
{
    Matrix A(4, 4);
    A(0, 0) = 3.;
    A(0, 1) = -9.;
    A(0, 2) = 7.;
    A(0, 3) = 3.;
    A(1, 0) = 3.;
    A(1, 1) = -8.;
    A(1, 2) = 2.;
    A(1, 3) = -9.;
    A(2, 0) = -4.;
    A(2, 1) = 4.;
    A(2, 2) = 4.;
    A(2, 3) = 1.;
    A(3, 0) = -6.;
    A(3, 1) = 5.;
    A(3, 2) = -1.;
    A(3, 3) = 1.;

    Matrix B(4, 4);
    B(0, 0) = 8.;
    B(0, 1) = 2.;
    B(0, 2) = 2.;
    B(0, 3) = 2.;
    B(1, 0) = 3.;
    B(1, 1) = -1.;
    B(1, 2) = 7.;
    B(1, 3) = 0.;
    B(2, 0) = 7.;
    B(2, 1) = 0.;
    B(2, 2) = 5.;
    B(2, 3) = 4.;
    B(3, 0) = 6.;
    B(3, 1) = -2.;
    B(3, 2) = 0.;
    B(3, 3) = 5.;

    Matrix C = A.Mul(B);
    CHECK(A == C.Mul(B.Inverse()));
}

TEST_CASE("Test matrix properties")
{
    Matrix I = Matrix::Identity(4);
    CHECK(I.Inverse() == I);

    Matrix A(4, 4);
    A(0, 0) = 3.;
    A(0, 1) = -9.;
    A(0, 2) = 7.;
    A(0, 3) = 3.;
    A(1, 0) = 3.;
    A(1, 1) = -8.;
    A(1, 2) = 2.;
    A(1, 3) = -9.;
    A(2, 0) = -4.;
    A(2, 1) = 4.;
    A(2, 2) = 4.;
    A(2, 3) = 1.;
    A(3, 0) = -6.;
    A(3, 1) = 5.;
    A(3, 2) = -1.;
    A(3, 3) = 1.;

    CHECK(A.Mul(A.Inverse()) == I);

    CHECK(A.Inverse().T() == A.T().Inverse());

    Matrix I2 = Matrix::Identity(4);
    I2(0, 1) = 2.;
    Tuple B = Tuple(4., 5., 6., 7.);

    CHECK(I2.Mul(B) != B);
}

TEST_CASE("Multiplying by a translation matrix")
{
    Matrix T = Matrix::Translation(5., -3., 2.);
    Point P(-3., 4., 5.);

    CHECK(T.Mul(P) == Point(2., 1., 7.));

    CHECK(P.Translate(5., -3., 2.) == Point(2., 1., 7.));
}

TEST_CASE("Multiplying by the inverse of a translation matrix")
{
    Matrix T = Matrix::Translation(5., -3., 2.);
    auto Inv = T.Inverse();
    Point P(-3., 4., 5.);

    CHECK(Inv.Mul(P) == Point(-8., 7., 3.));
}

TEST_CASE("Translation does not affect vectors")
{
    Matrix T = Matrix::Translation(5., -3., 2.);
    Vector V(-3., 4., 5.);

    CHECK(T.Mul(V) == V);
}

TEST_CASE("A scaling matrix applied to a point")
{
    Matrix S = Matrix::Scaling(2., 3., 4.);
    Point P(-4., 6., 8.);

    CHECK(S.Mul(P) == Point(-8., 18., 32.));
}

TEST_CASE("A scaling matrix applied to a vector")
{
    Matrix S = Matrix::Scaling(2., 3., 4.);
    Vector V(-4., 6., 8.);

    CHECK(S.Mul(V) == Vector(-8., 18., 32.));
}

TEST_CASE("Multiplying by the inverse of a scaling matrix")
{
    Matrix S = Matrix::Scaling(2., 3., 4.);
    Vector V(-4., 6., 8.);

    CHECK(S.Inverse().Mul(V) == Vector(-2., 2., 2.));
}

TEST_CASE("Reflection is scaling by a negative value")
{
    Matrix S = Matrix::Scaling(-1., 1., 1.);
    Point P(2., 3., 4.);

    CHECK(S.Mul(P) == Point(-2., 3., 4.));
}

TEST_CASE("Rotating a point around the x axis")
{
    Point P(0., 1., 0.);
    Matrix R4 = Matrix::RotationX(M_PI/4);
    Matrix R2 = Matrix::RotationX(M_PI/2);

    CHECK(R4.Mul(P) == Point(0., std::sqrt(2.)/2, std::sqrt(2.)/2));
    CHECK(R2.Mul(P) == Point(0., 0., 1.));
}

TEST_CASE("The inverse of an x-rotation rotate in the opposite direction")
{
    Point P(0., 1., 0.);
    Matrix R4 = Matrix::RotationX(M_PI/4);

    CHECK(R4.Inverse().Mul(P) == Point(0., std::sqrt(2.)/2, -std::sqrt(2.)/2));
}

TEST_CASE("Rotating a point around the x axis")
{
    Point P(0., 1., 0.);
    Matrix R4 = Matrix::RotationX(M_PI/4);
    Matrix R2 = Matrix::RotationX(M_PI/2);

    CHECK(R4.Mul(P) == Point(0., std::sqrt(2.)/2, std::sqrt(2.)/2));
    CHECK(R2.Mul(P) == Point(0., 0., 1.));
}

TEST_CASE("Rotating a point around the y axis")
{
    Point P(0., 0., 1.);
    Matrix R4 = Matrix::RotationY(M_PI/4);
    Matrix R2 = Matrix::RotationY(M_PI/2);

    CHECK(R4.Mul(P) == Point(std::sqrt(2.)/2, 0., std::sqrt(2.)/2));
    CHECK(R2.Mul(P) == Point(1., 0., 0.));
}

TEST_CASE("Rotating a point around the z axis")
{
    Point P(0., 1., 0.);
    Matrix R4 = Matrix::RotationZ(M_PI/4);
    Matrix R2 = Matrix::RotationZ(M_PI/2);

    CHECK(R4.Mul(P) == Point(-std::sqrt(2.)/2, std::sqrt(2.)/2, 0.));
    CHECK(R2.Mul(P) == Point(-1., 0., 0.));
}

TEST_CASE("A shearing transformation moves x in proportion to y")
{
    Point P(2., 3., 4.);
    Matrix S = Matrix::Shearing(1., 0., 0., 0., 0., 0.);

    CHECK(S.Mul(P) == Point(5., 3., 4.));
}

TEST_CASE("A shearing transformation moves x in proportion to z")
{
    Point P(2., 3., 4.);
    Matrix S = Matrix::Shearing(0., 1., 0., 0., 0., 0.);

    CHECK(S.Mul(P) == Point(6., 3., 4.));
}

TEST_CASE("A shearing transformation moves y in proportion to x")
{
    Point P(2., 3., 4.);
    Matrix S = Matrix::Shearing(0., 0., 1., 0., 0., 0.);

    CHECK(S.Mul(P) == Point(2., 5., 4.));
}

TEST_CASE("A shearing transformation moves y in proportion to z")
{
    Point P(2., 3., 4.);
    Matrix S = Matrix::Shearing(0., 0., 0., 1., 0., 0.);

    CHECK(S.Mul(P) == Point(2., 7., 4.));
}

TEST_CASE("A shearing transformation moves z in proportion to x")
{
    Point P(2., 3., 4.);
    Matrix S = Matrix::Shearing(0., 0., 0., 0., 1., 0.);

    CHECK(S.Mul(P) == Point(2., 3., 6.));
}

TEST_CASE("A shearing transformation moves z in proportion to y")
{
    Point P(2., 3., 4.);
    Matrix S = Matrix::Shearing(0., 0., 0., 0., 0., 1.);

    CHECK(S.Mul(P) == Point(2., 3., 7.));
}

TEST_CASE("Individual transformations are applied in sequence")
{
    Point P(1., 0., 1.);
    Matrix A = Matrix::RotationX(M_PI/2);
    Matrix B = Matrix::Scaling(5., 5., 5.);
    Matrix C = Matrix::Translation(10., 5., 7.);

    CHECK(C.Mul(B).Mul(A).Mul(P) == Point(15., 0., 7.));
    CHECK(C.Mul(B.Mul(A.Mul(P))) == Point(15., 0., 7.));

    CHECK(P.RotateX(M_PI/2).Scale(5., 5., 5.).Translate(10., 5., 7.) == Point(15., 0., 7.));
}