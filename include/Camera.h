#pragma once

#include "doctest.h"
#include "Matrix.h"
#include "Color.h"
#include "Canvas.h"
#include "World.h"
#include "Ray.h"

class Camera
{
    int HSize;
    int VSize;
    // FOV is in radian
    double FieldOfView;
    Matrix Transform;
    double PixelSize;
    double HalfWidth;
    double HalfHeight;

public:
    Camera();
    Camera(int H, int V, double FOV);

    void ComputePixelSize();

    inline int GetHSize() { return HSize; }
    inline int GetVSize() { return VSize; }
    inline double GetFOV() { return FieldOfView; }
    inline Matrix GetTransform() { return Transform; }
    inline double GetPixelSize() { return PixelSize; }

    inline void SetPixelSize(double PS) { PixelSize = PS; }
    inline void SetTransform(Matrix &M) { Transform = M; }
    inline void SetTransform(Matrix &&M) { Transform = M; }

    // RayForPixel returns a ray that starts at the camera passes through the 
    // indicated (X, Y) pixel on the canvas.
    Ray RayForPixel(int X, int Y);

    Canvas Render(World &W, bool RenderShadow, bool printLog, int RayDepth);
};