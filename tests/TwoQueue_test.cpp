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
