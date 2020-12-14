#include <condition_variable>
#include <functional>
#include <vector>
#include <thread>
#include <iostream>
#include <queue>
#include <chrono>
#include "threadpool.h"

int main()
{
    std::cout << "Hello, World!\n";
    std::cout << "hardware_concurrency(): " << std::thread::hardware_concurrency() << '\n';
    std::vector<int> testVector {0, 0, 0, 0, 0};

    {
        ThreadPool pool{std::thread::hardware_concurrency()};

        for (int i = 0; i < testVector.size(); ++i)
        {
            std::cout << testVector[i] << ' ';
        }
        std::cout << '\n';

        pool.enqueue([&] {
            testVector[0] = 1;
        });
        pool.enqueue([&] {
            testVector[2] = 1;
        });
    }

    for (int i = 0; i < testVector.size(); ++i)
    {
        std::cout << testVector[i] << ' ';
    }
    std::cout << '\n';

    return 0;
}