#include "include/Color.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "include/Canvas.h"

const int Canvas::MAX_LINE_LENGTH = 70;
const int Canvas::MAX_COLOR_VALUE = 255;

Canvas::Canvas() {}

Canvas::Canvas(int Width, int Height)
{
    width = Width;
    height = Height;
    // set default max color value to 255
    maxColorValue = 255;
    canvas = std::vector<std::vector<Color>>(Height, std::vector<Color>(Width, Color(0.f, 0.f, 0.f)));
}

Canvas::Canvas(int Width, int Height, Color C)
{
    width = Width;
    height = Height;
    // set default max color value to 255
    maxColorValue = 255;
    canvas = std::vector<std::vector<Color>>(Height, std::vector<Color>(Width, C));
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
    return (X >= 0) && (X < width) && (Y >= 0) && (Y < height);
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

    for (auto Row : canvas)
    {   
        int LineLength = 0;
        for (auto C : Row)
        {
            auto PPMVals = C.ToPPMVal(MAX_COLOR_VALUE);
            for (auto Val : PPMVals)
            {
                std::string ValStr = std::to_string(Val);
                if (LineLength == 0) {}
                else if (LineLength + ValStr.length() + 1 > MAX_LINE_LENGTH)
                {
                    s += '\n';
                    LineLength = 0;
                }
                else
                {
                    s += ' ';
                    LineLength += 1;
                }
                    
                s += ValStr;
                LineLength += ValStr.length();
            }
        }

        s += '\n';
    }

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

TEST_CASE("constructing the PPM pixel data")
{
    Canvas A = Canvas(5, 3);
    Color C1 = Color(1.5f, 0.f, 0.f);
    Color C2 = Color(0.f, 0.5f, 0.f);
    Color C3 = Color(-0.5f, 0.f, 1.f);

    A.WritePixel(0, 0, C1);
    A.WritePixel(2, 1, C2);
    A.WritePixel(4, 2, C3);

    const char *Expected = R"(P3
5 3
255
255 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 128 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 255
)";

    std::string Generated = A.ToPPM();
    CHECK(Generated == std::string(Expected));

    // check if last character in the file is a newline character
    CHECK(Generated[Generated.length()-1] == '\n');

    // split string by lines
    // auto result = std::vector<std::string>{};
    // std::stringstream ss(A.ToPPM());

    // for (std::string line; std::getline(ss, line, '\n');)
    //     result.push_back(line);
}

TEST_CASE("splitting long lines in PPM files")
{
    Canvas A = Canvas(10, 2, Color(1.f, 0.8f, 0.6f));
    const char *Expected = R"(P3
10 2
255
255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204
153 255 204 153 255 204 153 255 204 153 255 204 153
255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204
153 255 204 153 255 204 153 255 204 153 255 204 153
)";
    CHECK(A.ToPPM() == std::string(Expected));
}