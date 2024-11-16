#include "../include/TwoQueue.hpp"
#include <gtest/gtest.h>
#include <iostream>

// Example test for TwoQueue
TEST(TwoQueue, PutBackTest)
{
    TwoQueueConfig config = {
        3,   // Qin capacity
        3,   // Qout capacity
        3,   // Qmain capacity
    };
    TwoQueue<int, std::string> cache(config);

    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");

    // Check that Qout and Qmain are empty initially (assuming they should be)
    EXPECT_TRUE(cache.get_qout().empty());
    EXPECT_TRUE(cache.get_qmain().empty());

    // Check the contents of Qin
    std::vector<std::pair<int, std::string>> expected_qin = { { 1, "one" },
                                                              { 2, "two" },
                                                              { 3, "three" } };

    auto it = expected_qin.begin();
    for (const auto& item : cache.get_qin()) {
        EXPECT_EQ(item, it->second);   // Compare values
        ++it;
    }
}

// Example test for TwoQueue
TEST(TwoQueue, PutBackMany)
{
    TwoQueueConfig config = {
        3,   // Qin capacity
        3,   // Qout capacity
        3,   // Qmain capacity
    };
    TwoQueue<int, std::string> cache(config);

    for (int i = 1; i <= 6; i++) {
        cache.put(i, "val" + std::to_string(i));
    }

    // Check the contents of Qin
    std::vector<std::pair<int, std::string>> expected_qin = { { 4, "val4" },
                                                              { 5, "val5" },
                                                              { 6, "val6" } };

    auto it_qin = expected_qin.begin();
    for (const auto& item : cache.get_qin()) {
        EXPECT_EQ(item, it_qin->second);   // Compare values
        ++it_qin;
    }

    // Check the contents of Qout
    std::vector<std::pair<int, std::string>> expected_qout = { { 1, "val1" },
                                                               { 2, "val2" },
                                                               { 3, "val3" } };

    auto it_qout = expected_qout.begin();
    for (const auto& item : cache.get_qout()) {
        EXPECT_EQ(item, it_qout->second);   // Compare values
        ++it_qout;
    }

    // Adding elements that have to be pushed in 'Qmain'
    for (int i = 4; i <= 6; i++) {
        cache.put(i, "val" + std::to_string(i));
    }

    // Check the contents of Qmain
    std::vector<std::pair<int, std::string>> expected_qmain = { { 4, "val4" },
                                                                { 5, "val5" },
                                                                { 6, "val6" } };

    auto it_qmain = expected_qmain.begin();
    for (const auto& item : cache.get_qmain()) {
        EXPECT_EQ(item, it_qmain->second);   // Compare values
        ++it_qmain;
    }

    // Qin should be empty
    EXPECT_TRUE(cache.get_qin().empty());
    EXPECT_TRUE(cache.get_qout().size() == 3);
}

// Example test for TwoQueue
TEST(TwoQueue, Get)
{
    TwoQueueConfig config = { 5, 5, 5 };
    TwoQueue<int, std::string> cache(config);

    for (int i = 1; i <= 5; i++) {
        cache.put(i, "val" + std::to_string(i));
    }
}

// Test: Get non-existent key from empty cache
TEST(TwoQueue, GetFromEmptyCache)
{
    TwoQueueConfig config = { 3, 3, 3 };
    TwoQueue<int, std::string> cache(config);

    // Attempt to get a value from an empty cache
    auto value = cache.get(1);
    EXPECT_FALSE(value.has_value());   // Should return std::nullopt
}

TEST(TwoQueue, IteratorValidityAfterMove)
{
    TwoQueueConfig config = { 2, 2, 2 };   // Small capacities for easier testing
    TwoQueue<int, std::string> cache(config);

    // Add elements to Qin
    cache.put(1, "val1");
    cache.put(2, "val2");

    // Ensure elements are in Qin
    EXPECT_EQ(cache.get_qin().size(), 2);
    EXPECT_EQ(cache.get_qmain().size(), 0);
    EXPECT_EQ(cache.get_qout().size(), 0);

    // Check lookup for Qin
    auto lookup = cache.get_lookup();
    EXPECT_EQ(*lookup[1].it, "val1");
    EXPECT_EQ(*lookup[2].it, "val2");

    // Promote first element to Qmain
    cache.put(1, "val1");

    // Check element in Qmain
    EXPECT_EQ(cache.get_qmain().size(), 1);
    EXPECT_EQ(cache.get_qin().size(), 1);   // Qin should now have only "val2"
    EXPECT_EQ(*lookup[1].it, "val1");
    EXPECT_EQ(*cache.get_qin().begin(), "val2");
    EXPECT_EQ(*cache.get_qmain().begin(), "val1");

    // Promote the second element to Qmain
    cache.put(2, "val2");

    // Both elements should now be in Qmain
    EXPECT_EQ(cache.get_qmain().size(), 2);
    EXPECT_EQ(cache.get_qin().size(), 0);   // Qin should now be empty
    EXPECT_EQ(*lookup[2].it, "val2");
    EXPECT_EQ(*lookup[1].it, "val1");
}

TEST(TwoQueue, GetTwice)
{
    TwoQueueConfig config = { 2, 2, 2 };   // Small capacities for easier testing
    TwoQueue<int, std::string> cache(config);

    // Add elements to Qin
    cache.put(1, "val1");
    cache.put(2, "val2");

    cache.get(1);
    cache.get(1);

    auto result = cache.get(1);

    EXPECT_EQ(result->get(), "val1");
}
