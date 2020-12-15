#include "include/BoundingBoxes.h"
#include <limits>
// #include "include/Ray.h"
// #include "include/Intersection.h"
// #include "include/Util.h"
// #include "include/Sphere.h"
// #include "include/Transformations.h"
// #include "include/Functions.h"

BoundingBoxes::BoundingBoxes(int ID)
{
    Transform = Matrix::Identity();
    TransformInverse = Matrix::Identity();
    Origin = Point(0., 0., 0.);
    AMaterial = Material();
    UseShadow = true;
    this->ID = ID;

    double Inf = std::numeric_limits<double>::max();
    Min = Point(Inf, Inf, Inf);
    Max = Point(-Inf, -Inf, -Inf);
}

BoundingBoxes::BoundingBoxes() : BoundingBoxes(0)
{
}

BoundingBoxes::BoundingBoxes(int ID, Point Minimum, Point Maximum)
{
    Transform = Matrix::Identity();
    TransformInverse = Matrix::Identity();
    Origin = Point(0., 0., 0.);
    AMaterial = Material();
    UseShadow = true;
    this->ID = ID;

    Min = Minimum;
    Max = Maximum;
}

int BoundingBoxes::GetID()
{
    return ID;
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