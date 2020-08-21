#pragma once

#include "doctest.h"
#include "Color.h"
#include <iostream>
#include <vector>

class Canvas
{
private:
    int width, height;
    std::vector<std::vector<Color>> canvas;

public:
    Canvas();
    Canvas(int Width, int Height);

    int WritePixel(int X, int Y, Color C);
    Color *GetPixel(int X, int Y);
    bool ValidPixel(int X, int Y);

    int GetWidth();
    int GetHeight();
};