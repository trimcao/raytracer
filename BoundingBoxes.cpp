#include "include/BoundingBoxes.h"
#include <limits>
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

// BoundingBoxes BoundingBoxes::ParentSpaceBox(std::pair<Point, Point> ObjectBounds, Matrix TransformMatrix)
// {
//     BoundingBoxes Box(ObjectBounds);
//     return Box.TransformBox(TransformMatrix);
// }

// BoundingBoxes BoundingBoxes::BoundingBoxOf(Groups &G)
// {
//     BoundingBoxes Box;
//     for (auto &Child: G.GetShapes())
//     {
//         auto ChildBox = ParentSpaceBox(Child->BoundsOf(), Child->GetTransform());
//         Box.AddBox(ChildBox);
//     }
//     return Box;
// }

// BoundingBoxes BoundingBoxes::BoundingBoxOf(CSG &C)
// {
//     BoundingBoxes Box;
//     auto LeftChildBox = ParentSpaceBox(C.GetLeft()->BoundsOf(), C.GetLeft()->GetTransform());
//     Box.AddBox(LeftChildBox);
//     auto RightChildBox = ParentSpaceBox(C.GetRight()->BoundsOf(), C.GetRight()->GetTransform());
//     Box.AddBox(RightChildBox);
//     return Box;
// }