#include "include/doctest.h"
#include <iostream>
#include <exception>
#include <vector>
#include <string>
#include "include/Util.h"
#include "include/Matrix.h"

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