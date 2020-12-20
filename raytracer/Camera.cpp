#include "include/Camera.h"
#include "include/Util.h"
#include "include/Transformations.h"
// #include "include/Intersection.h"
#include <iostream>
#include <cmath>
#include <chrono>
#include "threadpool.h"

Camera::Camera(int H, int V, double FOV)
{
    HSize = H;
    VSize = V;
    FieldOfView = FOV;
    Transform = Matrix::Identity(4);
    TransformInverse = Matrix::Identity(4);
    ComputePixelSize();
}

void Camera::SetTransform(Matrix &M)
{
    Transform = M;
    TransformInverse = Transform.Inverse();
}

void Camera::ComputePixelSize()
{
    double HalfView = std::tan(FieldOfView / 2);
    double Aspect = (double)(HSize) / (double)(VSize);

    if (Aspect >= 1.)
    {
        HalfWidth = HalfView;
        HalfHeight = HalfView / Aspect;
    } else
    {
        HalfWidth = HalfView * Aspect;
        HalfHeight = HalfView;
    }

    SetPixelSize(HalfWidth * 2 / HSize);
}

Ray Camera::RayForPixel(int X, int Y)
{
    // the offset from the edge of the canvas to the pixel's center
    // 0.5 here means we want to find the pixel's center coordinate (half a pixel_size unit)
    auto XOffset = (X + 0.5) * PixelSize;
    auto YOffset = (Y + 0.5) * PixelSize;

    // the untransformed coordinates of the pixel in world space
    // (remember that the camera looks toward -z, so +x is to the *left*)
    auto WorldX = HalfWidth - XOffset;
    auto WorldY = HalfHeight - YOffset;

    // using the camera matrix, transform the canvas point and the origin
    // and then compute the ray's direction vector
    // (remember that the canvas is at z=-1)
    auto Pixel = TransformInverse.Mul(Point(WorldX, WorldY, -1.));
    auto Origin = TransformInverse.Mul(Point(0., 0., 0.));
    auto Direction = (Pixel - Origin).Normalize();

    return Ray(Origin, Direction);
}

Canvas Camera::Render(World &W, bool RenderShadow, bool printLog, int RayDepth, uint numThreads)
{
    Canvas Image(HSize, VSize);

    // variables for progress bar
    int TotalPixels = HSize * VSize;
    int CurPixel = 0;
    int Step = 1;
    int DisplayNext = 1;

    auto StartTime = std::chrono::system_clock::now();

    // Use a thread pool here
    ThreadPool pool{numThreads};

    for (int Y = 0; Y < VSize; ++Y)
    {
        for (int X = 0; X < HSize; ++X)
        {
            auto CurY = Y;
            auto CurX = X;

            // enqueue a task
            pool.enqueue([=, &W, &Image] {
                auto R = RayForPixel(CurX, CurY);
                auto Col = W.ColorAt(R, RenderShadow, RayDepth);
                // std::cout << "CurX-CurY: "  << CurX << '-' << CurY << "\n";
                // std::cout << "Ray: "  << R.GetDirection() << '-' << R.GetOrigin() << "\n";
                // std::cout << "Col: "  << Col << "\n";
                Image.WritePixel(CurX, CurY, Col);
            });

            // if (printLog)
            // {
            //     // Formatted progress indicator
            //     ++CurPixel;
            //     auto Percent = (100 * (CurPixel + 1)) / TotalPixels ;
            //     if (Percent >= DisplayNext)
            //     {
            //         std::cout << "\r" << "Progress [" << std::string(Percent / 5, '=') << std::string(100 / 5 - Percent / 5, ' ') << "]";
            //         std::cout << ' ' << Percent << "%";

            //         auto CurrentTime = std::chrono::system_clock::now();
            //         std::chrono::duration<double> ElapsedSeconds = CurrentTime - StartTime;

            //         std::cout << "    " << "Elapsed time: " << (int)ElapsedSeconds.count() << "s";
            //         std::cout.flush();
            //         DisplayNext += Step;
            //     }
            // }
        }
    }

    if (printLog)
        std::cout << std::endl;

    return Image;
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