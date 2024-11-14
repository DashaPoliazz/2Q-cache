#include "../include/TwoQueue.hpp"
#include <gtest/gtest.h>
#include <iostream>

// Example test for TwoQueue
TEST(TwoQueue, PushBackTest)
{
    TwoQueueConfig config = {
        3,   // Qin capacity
        3,   // Qout capacity
        3,   // Qmain capacity
    };
    TwoQueue<int> cache(config);

    cache.push(1);
    cache.push(2);
    cache.push(3);

    // Check that Qout and Qmain are empty initially (assuming they should be)
    EXPECT_TRUE(cache.get_qout().empty());
    EXPECT_TRUE(cache.get_qmain().empty());

    // Check the contents of Qin
    int counter = 1;
    for (const auto& item : cache.get_qin()) {
        EXPECT_EQ(item, counter);
        ++counter;
    }
}
// Example test for TwoQueue
TEST(TwoQueue, PushBackMany)
{
    TwoQueueConfig config = {
        3,   // Qin capacity
        3,   // Qout capacity
        3,   // Qmain capacity
    };
    TwoQueue<int> cache(config);

    for (int i = 1; i <= 6; i++) {
        cache.push(i);
    }

    // Check the contents of Qin
    int qin_counter = 4;
    for (const auto& item : cache.get_qin()) {
        EXPECT_EQ(item, qin_counter);
        ++qin_counter;
    }
    // Check the contents of Qout
    int qout_counter = 1;
    for (const auto& item : cache.get_qout()) {
        EXPECT_EQ(item, qout_counter);
        ++qout_counter;
    }

    // Adding elements that has to be pushed in 'Qmain'
    for (int i = 4; i <= 6; i++) {
        cache.push(i);
    }

    int qmain_counter = 4;
    for (const auto& item : cache.get_qmain()) {
        EXPECT_EQ(item, qmain_counter);
        ++qmain_counter;
    }

    // qin should be empty
    EXPECT_TRUE(cache.get_qin().empty());
    EXPECT_TRUE(cache.get_qout().size() == 3);
}
