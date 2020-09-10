#include "include/Camera.h"
#include "include/Util.h"
#include "include/Transformations.h"
#include "include/Intersection.h"
#include <cmath>

Camera::Camera(int H, int V, float FOV)
{
    HSize = H;
    VSize = V;
    FieldOfView = FOV;
    Transform = Matrix::Identity(4);
    ComputePixelSize();
}

void Camera::ComputePixelSize()
{
    float HalfView = std::tan(FieldOfView / 2);
    float Aspect = (float)(HSize) / (float)(VSize);

    if (Aspect >= 1.f)
    {
        HalfWidth = HalfView;
        HalfHeight = HalfView / Aspect;
    } else
    {
        HalfHeight = HalfView;
        HalfWidth = HalfView * Aspect;
    }

    SetPixelSize(HalfWidth * 2 / HSize);
}

Ray Camera::RayForPixel(int X, int Y)
{
    // the offset from the edge of the canvas to the pixel's center
    // 0.5 here means we want to find the pixel's center coordinate (half a pixel_size unit)
    auto XOffset = (X + 0.5f) * PixelSize;
    auto YOffset = (Y + 0.5f) * PixelSize;

    // the untransformed coordinates of the pixel in world space
    // (remember that the camera looks toward -z, so +x is to the *left*)
    auto WorldX = HalfWidth - XOffset;
    auto WorldY = HalfHeight - YOffset;

    // using the camera matrix, transform the canvas point and the origin
    // and then compute the ray's direction vector
    // (remember that the canvas is at z=-1)
    auto TransformInverse = Transform.Inverse();
    auto Pixel = TransformInverse.Mul(Point(WorldX, WorldY, -1.f));
    auto Origin = TransformInverse.Mul(Point(0.f, 0.f, 0.f));
    auto Direction = (Pixel - Origin).Normalize();

    return Ray(Origin, Direction);
}

Canvas Camera::Render(World &W)
{
    Canvas Image(HSize, VSize);
    for (int X = 0; X < VSize; ++X)
    {
        for (int Y = 0; Y < HSize; ++Y)
        {
            auto R = RayForPixel(X, Y);
            auto Col = ColorAt(W, R);
            Image.WritePixel(X, Y, Col);
        }
    }

    return Image;
}

TEST_CASE("Constructing a camera")
{
    Camera Cam(160, 120, M_PI/2);
    CHECK(Cam.GetHSize() == 160);
    CHECK(Cam.GetVSize() == 120);
    CHECK(Util::Equal(Cam.GetFOV(), M_PI/2));
    CHECK(Cam.GetTransform() == Matrix::Identity(4));
}

TEST_CASE("The pixel size of a horizontal canvas")
{
    Camera Cam(200, 125, M_PI/2);
    CHECK(Util::Equal(Cam.GetPixelSize(), 0.01f));
}

TEST_CASE("The pixel size of a vertical canvas")
{
    Camera Cam(125, 200, M_PI/2);
    CHECK(Util::Equal(Cam.GetPixelSize(), 0.01f));
}

TEST_CASE("Constructing a ray through the center of the canvas")
{
    Camera Cam(201, 101, M_PI/2);
    Ray R = Cam.RayForPixel(100, 50);
    CHECK(R.GetOrigin() == Point(0.f, 0.f, 0.f));
    CHECK(R.GetDirection() == Vector(0.f, 0.f, -1.f));
}

TEST_CASE("Constructing a ray through a corner of the canvas")
{
    Camera Cam(201, 101, M_PI/2);
    Ray R = Cam.RayForPixel(0, 0);
    CHECK(R.GetOrigin() == Point(0.f, 0.f, 0.f));
    CHECK(R.GetDirection() == Vector(0.66519f, 0.33259f, -0.66851f));
}

TEST_CASE("Constructing a ray when the camera is transformed")
{
    Camera Cam(201, 101, M_PI/2);
    Cam.SetTransform(Matrix::RotationY(M_PI/4).Mul(Matrix::Translation(0.f, -2.f, 5.f)));
    Ray R = Cam.RayForPixel(100, 50);
    CHECK(R.GetOrigin() == Point(0.f, 2.f, -5.f));
    CHECK(R.GetDirection() == Vector(std::sqrt(2)/2, 0.f, -std::sqrt(2)/2));
}

TEST_CASE("Rendering a world with a camera")
{
    auto W = World::DefaultWorld();
    Camera Cam(11, 11, M_PI/2);
    
    Point From(0.f, 0.f, -5.f);
    Point To(0.f, 0.f, 0.f);
    Vector Up(0.f, 1.f, 0.f);
    Cam.SetTransform(Transformations::ViewTransform(From, To, Up));

    auto Image = Cam.Render(W);
    CHECK(*Image.GetPixel(5, 5) == Color(0.38066f, 0.47583f, 0.2855f));
}