
#include "BoundingBoxes.h"
#include "Util.h"
#include "Transformations.h"
#include "Sphere.h"
#include "Plane.h"
#include "Cubes.h"
#include "Cylinders.h"
#include "Cones.h"
#include "Triangles.h"
#include "gtest/gtest.h"
#include <limits>
#include <memory>

TEST(BoundingBoxes, EmptyBoundingBox) {
    double Inf = std::numeric_limits<double>::max();

    BoundingBoxes Box;
    EXPECT_EQ(Box.Min, Point(Inf, Inf, Inf));
    EXPECT_EQ(Box.Max, Point(-Inf, -Inf, -Inf));
}

TEST(BoundingBoxes, BoundingBoxWithVolume) {
    BoundingBoxes Box(1, Point(-1., -2., -3.), Point(3., 2., 1.));
    EXPECT_EQ(Box.Min, Point(-1., -2., -3.));
    EXPECT_EQ(Box.Max, Point(3., 2., 1.));
}

TEST(BoundingBoxes, AddingPointsToEmptyBoundingBox) {
    BoundingBoxes Box;
    Box.AddPoint(Point(-5., 2., 0.));
    Box.AddPoint(Point(7., 0., -3.));
    EXPECT_EQ(Box.Min, Point(-5., 0., -3.));
    EXPECT_EQ(Box.Max, Point(7., 2., 0.));
}

TEST(BoundingBoxes, SphereBoundingBox) {
    std::shared_ptr<Object> Shape = std::make_shared<Sphere>(Sphere());
    BoundingBoxes Box(Shape->BoundsOf());
    EXPECT_EQ(Box.Min, Point(-1., -1., -1.));
    EXPECT_EQ(Box.Max, Point(1., 1., 1.));
}

TEST(BoundingBoxes, PlaneBoundingBox) {
    std::shared_ptr<Object> Shape = std::make_shared<Plane>(Plane());
    BoundingBoxes Box(Shape->BoundsOf());
    EXPECT_EQ(Box.Min, Point(-Util::Inf, 0., -Util::Inf));
    EXPECT_EQ(Box.Max, Point(Util::Inf, 0., Util::Inf));
}

TEST(BoundingBoxes, CubeBoundingBox) {
    std::shared_ptr<Object> Shape = std::make_shared<Cubes>(Cubes());
    BoundingBoxes Box(Shape->BoundsOf());
    EXPECT_EQ(Box.Min, Point(-1., -1., -1.));
    EXPECT_EQ(Box.Max, Point(1., 1., 1.));
}

TEST(BoundingBoxes, CylindersBoundingBox) {
    std::shared_ptr<Object> Shape = std::make_shared<Cylinders>(Cylinders());
    BoundingBoxes Box(Shape->BoundsOf());
    EXPECT_EQ(Box.Min, Point(-1., -Util::Inf, -1.));
    EXPECT_EQ(Box.Max, Point(1., Util::Inf, 1.));

    Shape = std::make_shared<Cylinders>(Cylinders(-5., 3.));
    BoundingBoxes Box2(Shape->BoundsOf());
    EXPECT_EQ(Box2.Min, Point(-1., -5., -1.));
    EXPECT_EQ(Box2.Max, Point(1., 3., 1.));
}

TEST(BoundingBoxes, ConesBoundingBox) {
    std::shared_ptr<Object> Shape = std::make_shared<Cones>(Cones());
    BoundingBoxes Box(Shape->BoundsOf());
    EXPECT_EQ(Box.Min, Point(-Util::Inf, -Util::Inf, -Util::Inf));
    EXPECT_EQ(Box.Max, Point(Util::Inf, Util::Inf, Util::Inf));

    Shape = std::make_shared<Cones>(Cones(-5., 3.));
    BoundingBoxes Box2(Shape->BoundsOf());
    EXPECT_EQ(Box2.Min, Point(-5., -5., -5.));
    EXPECT_EQ(Box2.Max, Point(5., 3., 5.));
}

TEST(BoundingBoxes, TrianglesBoundingBox) {
    Point P1(-3., 7., 2.);
    Point P2(6., 2., -4.);
    Point P3(2., -1., -1.);

    std::shared_ptr<Object> Shape = std::make_shared<Triangles>(Triangles(P1, P2, P3));
    BoundingBoxes Box(Shape->BoundsOf());
    EXPECT_EQ(Box.Min, Point(-3., -1., -4.));
    EXPECT_EQ(Box.Max, Point(6., 7., 2.));
}