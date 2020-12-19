#include "include/BoundingBoxes.h"
#include <limits>
#include "include/Cubes.h"
#include <algorithm>
// #include "include/Ray.h"
// #include "include/Intersection.h"
// #include "include/Util.h"
// #include "include/Sphere.h"
// #include "include/Transformations.h"
// #include "include/Functions.h"

BoundingBoxes::BoundingBoxes()
{
    Min = Point(Util::Inf, Util::Inf, Util::Inf);
    Max = Point(-Util::Inf, -Util::Inf, -Util::Inf);
}

BoundingBoxes::BoundingBoxes(Point Minimum, Point Maximum)
{
    Min = Minimum;
    Max = Maximum;
}

BoundingBoxes::BoundingBoxes(std::pair<Point, Point> MinMax)
{
    Min = MinMax.first;
    Max = MinMax.second;
}

void BoundingBoxes::AddPoint(Point P)
{
    if (P.X() < Min.X()) Min.SetX(P.X());
    if (P.Y() < Min.Y()) Min.SetY(P.Y());
    if (P.Z() < Min.Z()) Min.SetZ(P.Z());

    if (P.X() > Max.X()) Max.SetX(P.X());
    if (P.Y() > Max.Y()) Max.SetY(P.Y());
    if (P.Z() > Max.Z()) Max.SetZ(P.Z());
}

void BoundingBoxes::AddBox(BoundingBoxes &B)
{
    AddPoint(B.Min);
    AddPoint(B.Max);
}

bool BoundingBoxes::ContainsPoint(Point P)
{
    return ( P.X() >= Min.X() && P.X() <= Max.X() )
            && ( P.Y() >= Min.Y() && P.Y() <= Max.Y() )
            && ( P.Z() >= Min.Z() && P.Z() <= Max.Z() );
}

bool BoundingBoxes::ContainsBox(BoundingBoxes &B)
{
    return ContainsPoint(B.Min) && ContainsPoint(B.Max);
}

BoundingBoxes BoundingBoxes::Transform(Matrix &M)
{
    std::vector<Point> PS {
        Min,
        Point(Min.X(), Min.Y(), Max.Z()),
        Point(Min.X(), Max.Y(), Min.Z()),
        Point(Min.X(), Max.Y(), Max.Z()),
        Point(Max.X(), Min.Y(), Min.Z()),
        Point(Max.X(), Min.Y(), Max.Z()),
        Point(Max.X(), Max.Y(), Min.Z()),
        Max
    };

    BoundingBoxes NewBox;
    for (auto &P: PS)
    {
        NewBox.AddPoint(M.Mul(P));
    }

    return NewBox;
}

bool BoundingBoxes::Intersect(const Ray &R)
{
    auto XT = CheckAxis(R.GetOrigin().X(), R.GetDirection().X(), Min.X(), Max.X());
    auto YT = CheckAxis(R.GetOrigin().Y(), R.GetDirection().Y(), Min.Y(), Max.Y());
    auto ZT = CheckAxis(R.GetOrigin().Z(), R.GetDirection().Z(), Min.Z(), Max.Z());

    std::vector<double> TMinCandidates {XT[0], YT[0], ZT[0]};
    std::vector<double> TMaxCandidates {XT[1], YT[1], ZT[1]};
    auto TMin = *std::max_element(TMinCandidates.begin(), TMinCandidates.end());
    auto TMax = *std::min_element(TMaxCandidates.begin(), TMaxCandidates.end());

    if (TMin > TMax)
        return false;

    return true;
}

std::pair<BoundingBoxes, BoundingBoxes> BoundingBoxes::SplitBounds()
{
    // figure out the box's largest dimension
    auto Dx = Max.X() - Min.X();
    auto Dy = Max.Y() - Min.Y();
    auto Dz = Max.Z() - Min.Z();
    std::vector<double> Dxyz {Dx, Dy, Dz};

    auto Greatest = *max_element(std::begin(Dxyz), std::end(Dxyz));
    auto X0 = Min.X();
    auto Y0 = Min.Y();
    auto Z0 = Min.Z();
    auto X1 = Max.X();
    auto Y1 = Max.Y();
    auto Z1 = Max.Z();

    // adjust the points so that they lie on the dividing plane
    if (Greatest == Dx)
    {
        X0 = X0 + (Dx / 2.);
        X1 = X0;
    } else if (Greatest == Dy)
    {
        Y0 = Y0 + (Dy / 2.);
        Y1 = Y0;
    } else
    {
        Z0 = Z0 + (Dz / 2.);
        Z1 = Z0;
    }

    Point MidMin(X0, Y0, Z0);
    Point MidMax(X1, Y1, Z1);

    BoundingBoxes Left(Min, MidMax);
    BoundingBoxes Right(MidMin, Max);
    return std::pair<BoundingBoxes, BoundingBoxes> {Left, Right};
}