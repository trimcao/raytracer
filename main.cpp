#define DOCTEST_CONFIG_IMPLEMENT
// #define DOCTEST_CONFIG_DISABLE
#include <iostream>
#include <vector>
#include <string>
#include "include/main.h"
#include "include/Tuple.h"

int main(int argc, char **argv)
{
    // use doctest to run unit tests
    doctest::Context ctx;
    ctx.applyCommandLine(argc, argv);
    int res = ctx.run();
    if (ctx.shouldExit())
        return res;

    // main code
    int res_main = program();

    return res + res_main;
}

int program()
{
    std::vector<std::string> msg{"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};

    for (const std::string &word : msg)
    {
        std::cout << word << " ";
    }
    std::cout << "\n";

    Tuple A = Tuple(1.0, 1.5, 0.5, 1.2);
    Tuple B = Tuple(1.0, 1.5, 0.5, 1.2);
    Tuple C = Tuple(1.0, 1.5, 0.5, 1.15);

    std::cout << "A: " << A.X << " " << A.Y << " " << A.Z << " " << A.W << "\n";

    std::cout << (equal(A.X, C.X) && equal(A.Y, C.Y) && equal(A.Z, C.Z) && equal(A.W, C.W)) << "\n";

    return 0;
}
