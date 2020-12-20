#include <condition_variable>
#include <functional>
#include <vector>
#include <thread>
#include <iostream>
#include <queue>
#include <chrono>

class ThreadPool
{
public:
    using Task = std::function<void()>;

    explicit ThreadPool(std::size_t numberOfThreads)
    {
        numThreads = numberOfThreads;
        start();
    }

    ~ThreadPool()
    {
        stop();
    }

    void enqueue(Task task)
    {
        {
            std::unique_lock<std::mutex> lock{mEventMutex};
            mTasks.emplace(std::move(task));
        }

        mEventVar.notify_one();
    }

    void setTasks(std::queue<Task> newTasks)
    {
        mTasks = newTasks;
    }

    void start()
    {
        for (auto i = 0u; i < numThreads; ++i)
        {
            mThreads.emplace_back([=] {
                while (true)
                {
                    Task task;
                    {
                        std::unique_lock<std::mutex> lock{mEventMutex};
                        // the predicate below (the lambda function) is the predicate of the condition_variable
                        // it will keep waiting until the bool expression (predicate) is satisfied
                        // here it means the thread will wake up when either: mStopping is true, or there's a task needs to be done
                        mEventVar.wait(lock, [=] { return mStopping || !mTasks.empty(); });

                        // if stopping or there's no task, this thread (worker) will be done
                        if (mStopping && mTasks.empty())
                            break;

                        task = std::move(mTasks.front());
                        mTasks.pop();
                    }

                    task();
                }
            });
        }
    }

private:
    std::size_t numThreads;
    std::vector<std::thread> mThreads;
    std::condition_variable mEventVar;

    std::mutex mEventMutex;
    bool mStopping = false;

    std::queue<Task> mTasks;

    void stop() noexcept
    {
        {
            std::unique_lock<std::mutex> lock{mEventMutex};
            mStopping = true;
        }

        mEventVar.notify_all();
        for (auto &thread : mThreads)
        {
            thread.join();
        }
    }
};