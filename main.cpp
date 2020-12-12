// #define DOCTEST_CONFIG_IMPLEMENT
// #define DOCTEST_CONFIG_DISABLE
#include <iostream>
#include <vector>
#include <string>
#include "include/Tuple.h"
#include "include/Point.h"
#include "include/Vector.h"
#include "include/Util.h"
#include "include/main.h"

int main(int argc, char **argv)
{
    // use doctest to run unit tests
    // doctest::Context ctx;
    // ctx.applyCommandLine(argc, argv);
    // int res = ctx.run();
    // if (ctx.shouldExit())
    //     return res;

    // main code
    int res_main = program();

    return res_main;
}

int program()
{
    std::vector<std::string> msg{"Hello", "Ray Tracing", "World"};

    for (const std::string &word : msg)
    {
        std::cout << word << " ";
    }
    std::cout << "\n";

    return 0;
}