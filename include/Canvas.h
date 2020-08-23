#pragma once

#include "doctest.h"
class Canvas
{
private:
    int width, height;
    int maxColorValue;
    static const int MAX_LINE_LENGTH;
    static const int MAX_COLOR_VALUE;
    std::vector<std::vector<Color>> canvas;

public:
    Canvas();
    Canvas(int Width, int Height);
    Canvas(int Width, int Height, Color C);

    int WritePixel(int X, int Y, Color C);
    Color *GetPixel(int X, int Y);
    bool ValidPixel(int X, int Y);

    std::string ToPPM();

    int GetWidth();
    int GetHeight();
};