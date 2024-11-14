
#include "../include/CacheQueue.hpp"
#include <gtest/gtest.h>

// Example test for CacheQueue
TEST(CacheQueueTest, PushBackTest)
{
    CacheQueue<int> cache(3);
    cache.push_back(1);
    cache.push_back(2);
    cache.push_back(3);

    EXPECT_EQ(cache.size(), 3);

    cache.push_back(4);
    EXPECT_EQ(cache.size(), 3);
    EXPECT_EQ(cache.front(), 2);
}

TEST(CacheQueueTest, CapacityLimitTest)
{
    CacheQueue<int> cache(2);
    cache.push_back(10);
    cache.push_back(20);
    cache.push_back(30);

    EXPECT_EQ(cache.size(), 2);
    EXPECT_EQ(cache.front(), 20);
}
