#include "Groups.h"
#include "Ray.h"
#include "Intersection.h"
#include "Util.h"
#include "Sphere.h"
#include "Transformations.h"
#include "Functions.h"
#include "gtest/gtest.h"

TEST(Groups, IntersectingRayGroupMissBoundingBox)
{
    auto Child = std::make_shared<TestShape>(TestShape());
    std::shared_ptr<Object> ChildObj = std::shared_ptr<Object>(Child);
    auto Shape = std::make_shared<Groups>(Groups());
    Shape->AddChild(ChildObj);
    Ray R(Point(0., 0., -5.), Vector(0., 1., 0.));
    auto XS = Shape->Intersect(R);

    EXPECT_EQ(true, Child->SavedRay == nullptr);
}

TEST(Groups, IntersectingRayGroupHitBoundingBox)
{
    auto Child = std::make_shared<TestShape>(TestShape());
    std::shared_ptr<Object> ChildObj = std::shared_ptr<Object>(Child);
    auto Shape = std::make_shared<Groups>(Groups());
    Shape->AddChild(ChildObj);
    Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));
    auto XS = Shape->Intersect(R);

    EXPECT_EQ(true, Child->SavedRay != nullptr);
}
