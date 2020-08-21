#pragma once

#include "doctest.h"
#include "Color.h"
#include <iostream>
#include <vector>
#include <string>

class Canvas
{
private:
    int width, height;
    int maxColorValue;
    std::vector<std::vector<Color>> canvas;

public:
    Canvas();
    Canvas(int Width, int Height);

    int WritePixel(int X, int Y, Color C);
    Color *GetPixel(int X, int Y);
    bool ValidPixel(int X, int Y);

    std::string ToPPM();

    int GetWidth();
    int GetHeight();
};