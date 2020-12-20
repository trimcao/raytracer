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

TEST(Groups, PartitionGroupChildren)
{
    std::shared_ptr<Object> S1 = std::make_shared<Sphere>(Sphere());
    S1->SetTransform(Transformations::Translation(-2., 0., 0.));
    std::shared_ptr<Object> S2 = std::make_shared<Sphere>(Sphere());
    S2->SetTransform(Transformations::Translation(2., 0., 0.));
    std::shared_ptr<Object> S3 = std::make_shared<Sphere>(Sphere());

    auto G = std::make_shared<Groups>(Groups());
    G->AddChild(S1);
    G->AddChild(S2);
    G->AddChild(S3);

    auto Partitions = G->PartitionChildren();
    auto Left = Partitions.first;
    auto Right = Partitions.second;

    EXPECT_EQ(G.get(), S3->GetParent());
    EXPECT_NE(G.get(), S1->GetParent());
    EXPECT_NE(G.get(), S2->GetParent());
    EXPECT_EQ(Left.size(), 1);
    EXPECT_EQ(Right.size(), 1);
    EXPECT_EQ(Left[0], S1);
    EXPECT_EQ(Right[0], S2);
}

TEST(Groups, CreateSubgroup)
{
    std::shared_ptr<Object> S1 = std::make_shared<Sphere>(Sphere());
    std::shared_ptr<Object> S2 = std::make_shared<Sphere>(Sphere());
    std::shared_ptr<Object> G = std::make_shared<Groups>(Groups());

    std::vector<std::shared_ptr<Object>> List {S1, S2};
    G->MakeSubgroup(List);

    EXPECT_EQ(1, G->GetCount());
    EXPECT_EQ(G->GetChildren()[0].get(), S1->GetParent());
    EXPECT_EQ(G->GetChildren()[0].get(), S2->GetParent());
}

TEST(Groups, SubdividingGroupThatPartitions)
{
    std::shared_ptr<Object> S1 = std::make_shared<Sphere>(Sphere());
    S1->SetTransform(Transformations::Translation(-2., -2., 0.));
    std::shared_ptr<Object> S2 = std::make_shared<Sphere>(Sphere());
    S2->SetTransform(Transformations::Translation(-2., 2., 0.));
    std::shared_ptr<Object> S3 = std::make_shared<Sphere>(Sphere());
    S3->SetTransform(Transformations::Scaling(4., 4., 4.));

    std::shared_ptr<Groups> G = std::make_shared<Groups>(Groups());
    G->AddChild(S1);
    G->AddChild(S2);
    G->AddChild(S3);

    G->Divide(1);

    EXPECT_EQ(G->GetChildren()[0], S3);
    EXPECT_EQ(G->GetChildren()[1]->GetCount(), 2);
    auto Subgroup = G->GetChildren()[1];
    EXPECT_EQ(Subgroup->GetChildren()[0].get(), S1->GetParent());
    EXPECT_EQ(Subgroup->GetChildren()[1].get(), S2->GetParent());
}

TEST(Groups, SubdividingGroupWithoutPartitions)
{
    std::shared_ptr<Object> S1 = std::make_shared<Sphere>(Sphere());
    S1->SetTransform(Transformations::Translation(-2., 0., 0.));
    std::shared_ptr<Object> S2 = std::make_shared<Sphere>(Sphere());
    S2->SetTransform(Transformations::Translation(2., 1., 0.));
    std::shared_ptr<Object> S3 = std::make_shared<Sphere>(Sphere());
    S3->SetTransform(Transformations::Translation(2., -1., 0.));
    std::shared_ptr<Object> S4 = std::make_shared<Sphere>(Sphere());

    std::shared_ptr<Object> Subgroup = std::make_shared<Groups>(Groups());
    std::shared_ptr<Object> G = std::make_shared<Groups>(Groups());

    Subgroup->AddChild(S1);
    Subgroup->AddChild(S2);
    Subgroup->AddChild(S3);
    G->AddChild(Subgroup);
    G->AddChild(S4);

    G->Divide(3);

    EXPECT_EQ(G->GetChildren()[0], Subgroup);
    EXPECT_EQ(G->GetChildren()[1], S4);
    EXPECT_EQ(Subgroup->GetCount(), 2);
    EXPECT_EQ(Subgroup->GetChildren()[0].get(), S1->GetParent());
    EXPECT_EQ(Subgroup->GetChildren()[1].get(), S2->GetParent());
    EXPECT_EQ(Subgroup->GetChildren()[1].get(), S2->GetParent());
}