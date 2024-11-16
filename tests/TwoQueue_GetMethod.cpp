#include "../include/TwoQueue.hpp"
#include <gtest/gtest.h>

TEST(TwoQueue, GetElementFromQin)
{
    TwoQueueConfig config = { 2, 2, 2 };
    TwoQueue<int, std::string> cache(config);

    // Add element to Qin
    cache.put(1, "val1");

    // Verify element is in Qin
    EXPECT_EQ(cache.get_qin().size(), 1);
    EXPECT_EQ(cache.get_qmain().size(), 0);

    // Access element
    auto result = cache.get(1);

    // Verify element is now in Qmain
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->get(), "val1");
    EXPECT_EQ(cache.get_qin().size(), 0);
    EXPECT_EQ(cache.get_qmain().size(), 1);
    EXPECT_EQ(*cache.get_qmain().begin(), "val1");
}

TEST(TwoQueue, GetElementFromQout)
{
    TwoQueueConfig config = { 1, 2, 1 };
    TwoQueue<int, std::string> cache(config);

    // Add elements and promote one to Qmain
    cache.put(1, "val1");
    cache.put(2, "val2");   // Moves "val1" to Qout

    // Verify element is in Qout
    EXPECT_EQ(cache.get_qout().size(), 1);
    EXPECT_EQ(*cache.get_qout().begin(), "val1");

    // Access element
    auto result = cache.get(1);

    // Verify element is now in Qmain
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->get(), "val1");
    EXPECT_EQ(cache.get_qmain().size(), 1);
    EXPECT_EQ(*cache.get_qmain().begin(), "val1");
    EXPECT_EQ(cache.get_qout().size(), 0);
}

TEST(TwoQueue, GetElementFromQmain)
{
    TwoQueueConfig config = { 1, 1, 3 };
    TwoQueue<int, std::string> cache(config);

    // Add elements and promote them to Qmain
    cache.put(1, "val1");
    cache.get(1);   // Moves "val1" to Qmain
    cache.put(2, "val2");
    cache.get(2);   // Moves "val2" to Qmain

    // Access "val1" again
    auto result = cache.get(1);

    // Verify "val1" is now the most recently used in Qmain
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->get(), "val1");
    EXPECT_EQ(cache.get_qmain().size(), 2);
    auto it = cache.get_qmain().begin();
    EXPECT_EQ(*it++, "val2");
    EXPECT_EQ(*it++, "val1");
}

TEST(TwoQueue, GetNonExistentElement)
{
    TwoQueueConfig config = { 1, 1, 1 };
    TwoQueue<int, std::string> cache(config);

    // Try to access a non-existent element
    auto result = cache.get(1);

    // Verify result is std::nullopt
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(cache.get_qin().size(), 0);
    EXPECT_EQ(cache.get_qmain().size(), 0);
    EXPECT_EQ(cache.get_qout().size(), 0);
}

TEST(TwoQueue, GetEvictedFromQmain)
{
    TwoQueueConfig config = { 1, 2, 1 };
    TwoQueue<int, std::string> cache(config);

    // Add elements and promote them to Qmain
    cache.put(1, "val1");
    cache.put(2, "val2");   // Moves "val1" to Qout

    // Verify "val1" is in Qout
    EXPECT_EQ(cache.get_qout().size(), 1);
    EXPECT_EQ(*cache.get_qout().begin(), "val1");

    // Access "val1"
    auto result = cache.get(1);

    // Verify "val1" is back in Qmain
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->get(), "val1");
    EXPECT_EQ(cache.get_qmain().size(), 1);
    EXPECT_EQ(*cache.get_qmain().begin(), "val1");
    EXPECT_EQ(cache.get_qout().size(), 0);
}

TEST(TwoQueue, GetEvictedFromQout)
{
    TwoQueueConfig config = { 1, 1, 1 };
    TwoQueue<int, std::string> cache(config);

    // Add elements to fill Qout
    cache.put(1, "val1");
    cache.get(1);           // Moves "val1" to Qmain
    cache.put(2, "val2");   // Moves "val1" to Qout
    cache.put(3, "val3");   // Evicts "val1" from Qout

    cache.get(2);

    EXPECT_EQ(cache.get_qmain().front(), "val2");
    EXPECT_EQ(cache.get_qout().front(), "val1");

    cache.put(4, "val4");

    EXPECT_FALSE(cache.get(1));
}
