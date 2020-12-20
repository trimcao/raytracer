#include "CSG.h"
#include "Ray.h"
#include "Intersection.h"
#include "Util.h"
#include "Transformations.h"
#include "Functions.h"
#include "gtest/gtest.h"

TEST(Groups, IntersectingRayCSGMissBoundingBox)
{
    auto Left = std::make_shared<TestShape>(TestShape());
    std::shared_ptr<Object> LeftObj = std::shared_ptr<Object>(Left);
    auto Right = std::make_shared<TestShape>(TestShape());
    std::shared_ptr<Object> RightObj = std::shared_ptr<Object>(Right);
    auto Shape = std::make_shared<CSG>(CSG(CSGOp::Difference, LeftObj, RightObj));
    Ray R(Point(0., 0., -5.), Vector(0., 1., 0.));
    auto XS = Shape->Intersect(R);

    EXPECT_EQ(true, Left->SavedRay == nullptr);
    EXPECT_EQ(true, Right->SavedRay == nullptr);
}

TEST(Groups, IntersectingRayCSGHitBoundingBox)
{
    auto Left = std::make_shared<TestShape>(TestShape());
    std::shared_ptr<Object> LeftObj = std::shared_ptr<Object>(Left);
    auto Right = std::make_shared<TestShape>(TestShape());
    std::shared_ptr<Object> RightObj = std::shared_ptr<Object>(Right);
    auto Shape = std::make_shared<CSG>(CSG(CSGOp::Difference, LeftObj, RightObj));
    Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));
    auto XS = Shape->Intersect(R);

    EXPECT_EQ(true, Left->SavedRay != nullptr);
    EXPECT_EQ(true, Right->SavedRay != nullptr);
}
