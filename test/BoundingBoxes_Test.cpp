
#include "BoundingBoxes.h"
#include "Util.h"
#include "Transformations.h"
// #include <cmath>
#include "gtest/gtest.h"
#include <limits>

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