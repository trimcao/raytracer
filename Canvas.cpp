#include "include/Canvas.h"

Canvas::Canvas() {}

Canvas::Canvas(int Width, int Height)
{
    width = Width;
    height = Height;
    // set default max color value to 255
    maxColorValue = 255;
    canvas = std::vector<std::vector<Color>>(Height, std::vector<Color>(Width, Color(0.f, 0.f, 0.f)));
}

int Canvas::GetWidth()
{
    return width;
}

int Canvas::GetHeight()
{
    return height;
}

bool Canvas::ValidPixel(int X, int Y)
{
    return (X >= 0) && (X <= width) && (Y >= 0) && (Y <= height);
}

int Canvas::WritePixel(int X, int Y, Color C)
{
    if (ValidPixel(X, Y))
    {
        canvas[Y][X] = C;
        return 0;
    }

    return 1;
}

Color *Canvas::GetPixel(int X, int Y)
{
    if (ValidPixel(X, Y))
        return &canvas[Y][X];
    return nullptr;
}

std::string Canvas::ToPPM()
{
    std::string s = "P3\n";
    s += std::to_string(width) + ' ';
    s += std::to_string(height) + '\n';
    s += "255\n";

    return s;
}

TEST_CASE("creating a canvas")
{
    Canvas A = Canvas(10, 20);
    CHECK(A.GetWidth() == 10);
    CHECK(A.GetHeight() == 20);
    CHECK(A.GetWidth() == 10);
}

TEST_CASE("writing pixels to a canvas")
{
    Canvas A = Canvas(10, 20);
    Color Red = Color(1.f, 0.f, 0.f);
    A.WritePixel(2, 3, Red);
    CHECK(*A.GetPixel(2, 3) == Color(1.f, 0.f, 0.f));
}