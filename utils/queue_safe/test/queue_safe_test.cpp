//
// Created by sauci on 21/11/2018.
//

#include <atomic>
#include <thread>
#include <condition_variable>
#include <gtest/gtest.h>
#include <mutex>

#include "queue_safe.h"

std::condition_variable cv;
std::mutex mutex;
std::atomic<bool> flag;

class Runner : public ::testing::Test
{
};

void calling_signal_on_int_queue_sets_signal_flag(Queue<int> *queue)
{
    bool signal = false;

    while (true)
    {
        queue->pop(signal);

        if (signal)
        {
            flag = true;

            break;
        }
    }
}

TEST(signal_method, calling_signal_on_int_queue_sets_signal_flag)
{
    flag = false;

    auto queue = new Queue<int>();
    auto thread = std::thread(calling_signal_on_int_queue_sets_signal_flag, queue);
    queue->signal();
    thread.join();

    ASSERT_EQ(flag, true);
}

void calling_signal_on_char_pointer_queue_sets_signal_flag(Queue<char *> *queue)
{
    bool signal = false;

    while (true)
    {
        queue->pop(signal);

        if (signal)
        {
            flag = true;

            break;
        }
    }
}

TEST(signal_method, calling_signal_on_char_pointer_queue_sets_signal_flag)
{
    flag = false;

    auto queue = new Queue<char *>();
    auto thread = std::thread(calling_signal_on_char_pointer_queue_sets_signal_flag, queue);
    queue->signal();
    thread.join();

    ASSERT_EQ(flag, true);
}

TEST(empty_method, calling_empty_on_empty_queue_returns_true)
{
    auto queue = new Queue<int>();
    GTEST_ASSERT_EQ(queue->empty(), true);
}

TEST(empty_method, calling_empty_on_non_empty_queue_returns_false)
{
    auto queue = new Queue<int>();
    queue->push(10);
    GTEST_ASSERT_EQ(queue->empty(), false);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
