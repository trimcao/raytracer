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
    float FieldOfView;
    Matrix Transform;
    float PixelSize;
    float HalfWidth;
    float HalfHeight;

public:
    Camera();
    Camera(int H, int V, float FOV);

    void ComputePixelSize();

    inline int GetHSize() { return HSize; }
    inline int GetVSize() { return VSize; }
    inline float GetFOV() { return FieldOfView; }
    inline Matrix GetTransform() { return Transform; }
    inline float GetPixelSize() { return PixelSize; }

    inline void SetPixelSize(float PS) { PixelSize = PS; }
    inline void SetTransform(Matrix &M) { Transform = M; }
    inline void SetTransform(Matrix &&M) { Transform = M; }

    // RayForPixel returns a ray that starts at the camera passes through the 
    // indicated (X, Y) pixel on the canvas.
    Ray RayForPixel(int X, int Y);

    Canvas Render(World &W, bool printLog);
};