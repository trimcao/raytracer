#pragma once

class Matrix
{
private:
    int numRows, numCols;
    std::vector<std::vector<float>> m;

public:
    Matrix();
    Matrix(int NumRows, int NumCols, float Val);
    Matrix(int NumRows, int NumCols);

    float &operator()(const int R, const int C) { return m[R][C]; }

    inline float At(const int R, const int C) const { return m[R][C]; }

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

    int GetNumRows() const;
    int GetNumCols() const;

};

bool operator==(const Matrix &LHS, const Matrix &RHS);

inline bool operator!=(const Matrix &LHS, const Matrix &RHS)
{
    return (!(LHS == RHS));
}

std::ostream &operator<<(std::ostream &os, const Matrix &M);