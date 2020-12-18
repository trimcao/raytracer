
#include "BoundingBoxes.h"
#include "Util.h"
#include "Transformations.h"
#include "Sphere.h"
#include "Plane.h"
#include "Cubes.h"
#include "Cylinders.h"
#include "Cones.h"
#include "Triangles.h"
#include "Groups.h"
#include "CSG.h"
#include "gtest/gtest.h"
#include <limits>
#include <memory>

TEST(BoundingBoxes, EmptyBoundingBox)
{
    double Inf = std::numeric_limits<double>::max();

    BoundingBoxes Box;
    EXPECT_EQ(Box.Min, Point(Inf, Inf, Inf));
    EXPECT_EQ(Box.Max, Point(-Inf, -Inf, -Inf));
}

TEST(BoundingBoxes, BoundingBoxWithVolume)
{
    BoundingBoxes Box(Point(-1., -2., -3.), Point(3., 2., 1.));
    EXPECT_EQ(Box.Min, Point(-1., -2., -3.));
    EXPECT_EQ(Box.Max, Point(3., 2., 1.));
}

TEST(BoundingBoxes, AddingPointsToEmptyBoundingBox)
{
    BoundingBoxes Box;
    Box.AddPoint(Point(-5., 2., 0.));
    Box.AddPoint(Point(7., 0., -3.));
    EXPECT_EQ(Box.Min, Point(-5., 0., -3.));
    EXPECT_EQ(Box.Max, Point(7., 2., 0.));
}

TEST(BoundingBoxes, SphereBoundingBox)
{
    std::shared_ptr<Object> Shape = std::make_shared<Sphere>(Sphere());
    BoundingBoxes Box(Shape->BoundsOf());
    EXPECT_EQ(Box.Min, Point(-1., -1., -1.));
    EXPECT_EQ(Box.Max, Point(1., 1., 1.));
}

TEST(BoundingBoxes, PlaneBoundingBox)
{
    std::shared_ptr<Object> Shape = std::make_shared<Plane>(Plane());
    BoundingBoxes Box(Shape->BoundsOf());
    EXPECT_EQ(Box.Min, Point(-Util::Inf, 0., -Util::Inf));
    EXPECT_EQ(Box.Max, Point(Util::Inf, 0., Util::Inf));
}

TEST(BoundingBoxes, CubeBoundingBox)
{
    std::shared_ptr<Object> Shape = std::make_shared<Cubes>(Cubes());
    BoundingBoxes Box(Shape->BoundsOf());
    EXPECT_EQ(Box.Min, Point(-1., -1., -1.));
    EXPECT_EQ(Box.Max, Point(1., 1., 1.));
}

TEST(BoundingBoxes, CylindersBoundingBox)
{
    std::shared_ptr<Object> Shape = std::make_shared<Cylinders>(Cylinders());
    BoundingBoxes Box(Shape->BoundsOf());
    EXPECT_EQ(Box.Min, Point(-1., -Util::Inf, -1.));
    EXPECT_EQ(Box.Max, Point(1., Util::Inf, 1.));

    Shape = std::make_shared<Cylinders>(Cylinders(-5., 3.));
    BoundingBoxes Box2(Shape->BoundsOf());
    EXPECT_EQ(Box2.Min, Point(-1., -5., -1.));
    EXPECT_EQ(Box2.Max, Point(1., 3., 1.));
}

TEST(BoundingBoxes, ConesBoundingBox)
{
    std::shared_ptr<Object> Shape = std::make_shared<Cones>(Cones());
    BoundingBoxes Box(Shape->BoundsOf());
    EXPECT_EQ(Box.Min, Point(-Util::Inf, -Util::Inf, -Util::Inf));
    EXPECT_EQ(Box.Max, Point(Util::Inf, Util::Inf, Util::Inf));

    Shape = std::make_shared<Cones>(Cones(-5., 3.));
    BoundingBoxes Box2(Shape->BoundsOf());
    EXPECT_EQ(Box2.Min, Point(-5., -5., -5.));
    EXPECT_EQ(Box2.Max, Point(5., 3., 5.));
}

TEST(BoundingBoxes, TrianglesBoundingBox)
{
    Point P1(-3., 7., 2.);
    Point P2(6., 2., -4.);
    Point P3(2., -1., -1.);

    std::shared_ptr<Object> Shape = std::make_shared<Triangles>(Triangles(P1, P2, P3));
    BoundingBoxes Box(Shape->BoundsOf());
    EXPECT_EQ(Box.Min, Point(-3., -1., -4.));
    EXPECT_EQ(Box.Max, Point(6., 7., 2.));
}

TEST(BoundingBoxes, AddingOneBoundingBoxToAnother)
{
    BoundingBoxes Box1(Point(-5., -2., -1.), Point(7., 4., 4.));
    BoundingBoxes Box2(Point(8., -7., -2.), Point(14., 2., 8.));
    Box1.AddBox(Box2);
    EXPECT_EQ(Box1.Min, Point(-5., -7., -2.));
    EXPECT_EQ(Box1.Max, Point(14., 4., 8.));
}

TEST(BoundingBoxes, BoxContainsPoint)
{
    BoundingBoxes Box(Point(5., -2., 0.), Point(11., 4., 7.));
    Point P;
    P = Point(5., -2., 0.);
    EXPECT_EQ(true, Box.ContainsPoint(P));
    P = Point(11., 4., 7.);
    EXPECT_EQ(true, Box.ContainsPoint(P));
    P = Point(8., 1., 3.);
    EXPECT_EQ(true, Box.ContainsPoint(P));
    P = Point(3., 0., 3.);
    EXPECT_EQ(false, Box.ContainsPoint(P));
    P = Point(8., -4., 3.);
    EXPECT_EQ(false, Box.ContainsPoint(P));
    P = Point(8., 1., -1.);
    EXPECT_EQ(false, Box.ContainsPoint(P));
    P = Point(13., 1., 3.);
    EXPECT_EQ(false, Box.ContainsPoint(P));
    P = Point(8., 5., 3.);
    EXPECT_EQ(false, Box.ContainsPoint(P));
    P = Point(8., 1., 8.);
    EXPECT_EQ(false, Box.ContainsPoint(P));
}

TEST(BoundingBoxes, BoxContainsBox)
{
    BoundingBoxes Box(Point(5., -2., 0.), Point(11., 4., 7.));
    BoundingBoxes Box2;

    Box2 = BoundingBoxes(Point(5., -2., 0.), Point(11., 4., 7.));
    EXPECT_EQ(true, Box.ContainsBox(Box2));
    Box2 = BoundingBoxes(Point(6., -1., 1.), Point(10., 3., 6.));
    EXPECT_EQ(true, Box.ContainsBox(Box2));
    Box2 = BoundingBoxes(Point(4., -3., -1.), Point(10., 3., 6.));
    EXPECT_EQ(false, Box.ContainsBox(Box2));
    Box2 = BoundingBoxes(Point(6., -1., 1.), Point(12., 5., 8.));
    EXPECT_EQ(false, Box.ContainsBox(Box2));
}

TEST(BoundingBoxes, TransformingBox)
{
    BoundingBoxes Box(Point(-1, -1., -1.), Point(1., 1., 1.));
    auto M = Transformations::RotationX(M_PI / 4).Mul(Transformations::RotationY(M_PI / 4));

    auto Box2 = Box.Transform(M);
    EXPECT_EQ(Box2.Min, Point(-1.41421, -1.70711, -1.70711));
    EXPECT_EQ(Box2.Max, Point(1.41421, 1.70711, 1.70711));
}

TEST(BoundingBoxes, ParentSpaceBounds)
{
    std::shared_ptr<Object> Shape = std::make_shared<Sphere>(Sphere());
    Shape->SetTransform(Transformations::Translation(1., -3., 5.).Mul(Transformations::Scaling(0.5, 2., 4.)));
    auto Box = Shape->ParentSpaceBoundsOf();
    EXPECT_EQ(Box.Min, Point(0.5, -5., 1.));
    EXPECT_EQ(Box.Max, Point(1.5, -1., 9.));
}

TEST(BoundingBoxes, GroupBoundingBox)
{
    std::shared_ptr<Object> S = std::make_shared<Sphere>(Sphere());
    S->SetTransform(Transformations::Translation(2., 5., -3.).Mul(Transformations::Scaling(2., 2., 2.)));

    std::shared_ptr<Object> C = std::make_shared<Cylinders>(Cylinders(-2., 2.));
    C->SetTransform(Transformations::Translation(-4., -1., 4.).Mul(Transformations::Scaling(0.5, 1., 0.5)));

    auto Shape = std::make_shared<Groups>(Groups());
    Shape->AddChild(S);
    Shape->AddChild(C);

    auto Box = Shape->BoundsOf();
    EXPECT_EQ(Box.Min, Point(-4.5, -3., -5.));
    EXPECT_EQ(Box.Max, Point(4., 7., 4.5));
}

TEST(BoundingBoxes, CSGBoundingBox)
{
    std::shared_ptr<Object> Left = std::make_shared<Sphere>(Sphere());
    std::shared_ptr<Object> Right = std::make_shared<Sphere>(Sphere());
    Right->SetTransform(Transformations::Translation(2., 3., 4.));

    auto Shape = std::make_shared<CSG>(CSG(CSGOp::Difference, Left, Right));

    auto Box = Shape->BoundsOf();
    EXPECT_EQ(Box.Min, Point(-1., -1., -1.));
    EXPECT_EQ(Box.Max, Point(3., 4., 5.));
}

TEST(BoundingBoxes, BoxIntersectAtOrigin)
{
    BoundingBoxes Box {Point(-1, -1, -1.), Point(1, 1, 1.)};
    Point Origin;
    Vector Direction;
    Ray R;

    Origin = Point(5., 0.5, 0.);
    Direction = Vector(-1., 0., 0.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(true, Box.Intersect(R));

    Origin = Point(-5., 0.5, 0.);
    Direction = Vector(1., 0., 0.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(true, Box.Intersect(R));

    Origin = Point(0.5, 5., 0.);
    Direction = Vector(0., -1., 0.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(true, Box.Intersect(R));

    Origin = Point(0.5, -5., 0.);
    Direction = Vector(0., 1., 0.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(true, Box.Intersect(R));

    Origin = Point(0.5, 0., 5.);
    Direction = Vector(0., 0., -1.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(true, Box.Intersect(R));

    Origin = Point(0.5, 0., -5.);
    Direction = Vector(0., 0., 1.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(true, Box.Intersect(R));

    Origin = Point(0., 0.5, 0.);
    Direction = Vector(0., 0., 1.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(true, Box.Intersect(R));

    Origin = Point(-2., 0., 0.);
    Direction = Vector(2., 4., 6.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(false, Box.Intersect(R));

    Origin = Point(0., -2., 0.);
    Direction = Vector(6., 2., 4.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(false, Box.Intersect(R));

    Origin = Point(0., 0., -2.);
    Direction = Vector(4., 6., 2.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(false, Box.Intersect(R));

    Origin = Point(2., 0., 2.);
    Direction = Vector(0., 0., -1.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(false, Box.Intersect(R));

    Origin = Point(0., 2., 2.);
    Direction = Vector(0., -1., 0.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(false, Box.Intersect(R));

    Origin = Point(2., 2., 0.);
    Direction = Vector(-1., 0., 0.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(false, Box.Intersect(R));
}

TEST(BoundingBoxes, BoxIntersectNonCubic)
{
    BoundingBoxes Box {Point(5., -2., 0.), Point(11, 4., 7.)};
    Point Origin;
    Vector Direction;
    Ray R;

    Origin = Point(15., 1., 2.);
    Direction = Vector(-1., 0., 0.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(true, Box.Intersect(R));

    Origin = Point(-5., -1., 4.);
    Direction = Vector(1., 0., 0.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(true, Box.Intersect(R));

    Origin = Point(7., 6., 5.);
    Direction = Vector(0., -1., 0.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(true, Box.Intersect(R));

    Origin = Point(9., -5., 6.);
    Direction = Vector(0., 1., 0.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(true, Box.Intersect(R));

    Origin = Point(8, 2., 12.);
    Direction = Vector(0., 0., -1.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(true, Box.Intersect(R));

    Origin = Point(6., 0., -5.);
    Direction = Vector(0., 0., 1.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(true, Box.Intersect(R));

    Origin = Point(8., 1., 3.5);
    Direction = Vector(0., 0., 1.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(true, Box.Intersect(R));

    Origin = Point(9., -1., -8.);
    Direction = Vector(2., 4., 6.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(false, Box.Intersect(R));

    Origin = Point(8., 3., -4.);
    Direction = Vector(6., 2., 4.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(false, Box.Intersect(R));

    Origin = Point(9., -1., -2.);
    Direction = Vector(4., 6., 2.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(false, Box.Intersect(R));

    Origin = Point(4., 0., 9.);
    Direction = Vector(0., 0., -1.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(false, Box.Intersect(R));

    Origin = Point(8., 6., -1.);
    Direction = Vector(0., -1., 0.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(false, Box.Intersect(R));

    Origin = Point(12., 5., 4.);
    Direction = Vector(-1., 0., 0.).Normalize();
    R = Ray(Origin, Direction);
    EXPECT_EQ(false, Box.Intersect(R));
}
