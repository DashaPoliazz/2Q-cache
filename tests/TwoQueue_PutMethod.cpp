#include "../include/TwoQueue.hpp"
#include <gtest/gtest.h>

TEST(TwoQueue, AddToQinAndEvictToQout)
{
    // Small capacities for easier testing
    TwoQueueConfig config = { 2, 2, 2 };
    TwoQueue<int, std::string> cache(config);

    // Add elements to Qin
    cache.put(1, "val1");
    cache.put(2, "val2");

    // Verify elements are in Qin
    EXPECT_EQ(cache.get_qin().size(), 2);
    EXPECT_EQ(cache.get_qout().size(), 0);
    EXPECT_EQ(cache.get_qmain().size(), 0);

    // Add a new element to trigger eviction from Qin to Qout
    cache.put(3, "val3");

    // Verify the state of the queues
    EXPECT_EQ(cache.get_qin().size(), 2);     // "val2" and "val3" in Qin
    EXPECT_EQ(cache.get_qout().size(), 1);    // "val1" moved to Qout
    EXPECT_EQ(cache.get_qmain().size(), 0);   // Qmain is still empty

    // Verify elements in Qin
    auto qin = cache.get_qin();
    auto it = qin.begin();
    EXPECT_EQ(*it++, "val2");
    EXPECT_EQ(*it++, "val3");

    // Verify elements in Qout
    auto qout = cache.get_qout();
    EXPECT_EQ(*qout.begin(), "val1");

    // Verify lookup consistency
    auto lookup = cache.get_lookup();
    EXPECT_EQ(lookup.size(), 3);
    EXPECT_EQ(lookup[1].queue_type, QueueType::Qout);
    EXPECT_EQ(lookup[2].queue_type, QueueType::Qin);
    EXPECT_EQ(lookup[3].queue_type, QueueType::Qin);

    // Verify reverse lookup (optional)
    EXPECT_TRUE(lookup.find(1) != lookup.end());
    EXPECT_TRUE(lookup.find(2) != lookup.end());
    EXPECT_TRUE(lookup.find(3) != lookup.end());
}

TEST(TwoQueue, AddToQmainByAccess)
{
    // Small capacities for easier testing
    TwoQueueConfig config = { 2, 2, 2 };
    TwoQueue<int, std::string> cache(config);

    // Add elements to Qin
    cache.put(1, "val1");
    cache.put(2, "val2");

    // Access an element to promote it to Qmain
    cache.get(1);

    // Verify the state of the queues
    EXPECT_EQ(cache.get_qmain().size(), 1);   // "val1" is in Qmain
    EXPECT_EQ(cache.get_qin().size(), 1);     // "val2" is still in Qin
    EXPECT_EQ(cache.get_qout().size(), 0);    // Qout is empty

    // Verify elements in Qmain
    auto qmain = cache.get_qmain();
    EXPECT_EQ(*qmain.begin(), "val1");

    // Verify elements in Qin
    auto qin = cache.get_qin();
    EXPECT_EQ(*qin.begin(), "val2");

    // Verify lookup consistency
    auto lookup = cache.get_lookup();
    EXPECT_EQ(lookup.size(), 2);
    EXPECT_EQ(lookup[1].queue_type, QueueType::Qmain);   // "val1" promoted to Qmain
    EXPECT_EQ(lookup[2].queue_type, QueueType::Qin);     // "val2" remains in Qin
}
