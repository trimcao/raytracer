#include "Camera.h"
#include "Util.h"
#include "Transformations.h"
#include <iostream>
#include <cmath>
#include <chrono>
#include "gtest/gtest.h"

TEST(Camera, ConstructingACamera) {
  Camera Cam(160, 120, M_PI/2);
  EXPECT_EQ(Cam.GetHSize(), 160);
  EXPECT_EQ(Cam.GetVSize(), 120);
  EXPECT_EQ(true, Util::Equal(Cam.GetFOV(), M_PI/2));
  EXPECT_EQ(Cam.GetTransform(), Matrix::Identity(4));
}

// TEST_CASE("Constructing a camera")
// {
//     Camera Cam(160, 120, M_PI/2);
//     CHECK(Cam.GetHSize() == 160);
//     CHECK(Cam.GetVSize() == 120);
//     CHECK(Util::Equal(Cam.GetFOV(), M_PI/2));
//     CHECK(Cam.GetTransform() == Matrix::Identity(4));
// }

// TEST_CASE("The pixel size of a horizontal canvas")
// {
//     Camera Cam(200, 125, M_PI/2);
//     CHECK(Util::Equal(Cam.GetPixelSize(), 0.01));
// }

// TEST_CASE("The pixel size of a vertical canvas")
// {
//     Camera Cam(125, 200, M_PI/2);
//     CHECK(Util::Equal(Cam.GetPixelSize(), 0.01));
// }

// TEST_CASE("Constructing a ray through the center of the canvas")
// {
//     Camera Cam(201, 101, M_PI/2);
//     Ray R = Cam.RayForPixel(100, 50);
//     CHECK(R.GetOrigin() == Point(0., 0., 0.));
//     CHECK(R.GetDirection() == Vector(0., 0., -1.));
// }

// TEST_CASE("Constructing a ray through a corner of the canvas")
// {
//     Camera Cam(201, 101, M_PI/2);
//     Ray R = Cam.RayForPixel(0, 0);
//     CHECK(R.GetOrigin() == Point(0., 0., 0.));
//     CHECK(R.GetDirection() == Vector(0.66519, 0.33259, -0.66851));
// }

// TEST_CASE("Constructing a ray when the camera is transformed")
// {
//     Camera Cam(201, 101, M_PI/2);
//     Cam.SetTransform(Matrix::RotationY(M_PI/4).Mul(Matrix::Translation(0., -2., 5.)));
//     Ray R = Cam.RayForPixel(100, 50);
//     CHECK(R.GetOrigin() == Point(0., 2., -5.));
//     CHECK(R.GetDirection() == Vector(std::sqrt(2)/2, 0., -std::sqrt(2)/2));
// }

// TEST_CASE("Rendering a world with a camera")
// {
//     auto W = World::DefaultWorld();
//     Camera Cam(11, 11, M_PI/2);
    
//     Point From(0., 0., -5.);
//     Point To(0., 0., 0.);
//     Vector Up(0., 1., 0.);
//     Cam.SetTransform(Transformations::ViewTransform(From, To, Up));

//     auto Image = Cam.Render(W);
//     CHECK(*Image.GetPixel(5, 5) == Color(0.38066, 0.47583, 0.2855));
// }