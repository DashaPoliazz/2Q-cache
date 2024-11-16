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

TEST(TwoQueue, MoveElementBetweenQueues)
{
    // Small capacities for easier testing
    TwoQueueConfig config = { 1, 1, 1 };
    TwoQueue<int, std::string> cache(config);

    // Step 1: Add an element to Qin
    cache.put(1, "val1");

    // Verify element is in Qin
    EXPECT_EQ(cache.get_qin().size(), 1);
    EXPECT_EQ(cache.get_qmain().size(), 0);
    EXPECT_EQ(cache.get_qout().size(), 0);
    EXPECT_EQ(*cache.get_qin().begin(), "val1");

    // Step 2: Access the element to promote it to Qmain
    cache.get(1);

    // Verify element is now in Qmain
    EXPECT_EQ(cache.get_qin().size(), 0);
    EXPECT_EQ(cache.get_qmain().size(), 1);
    EXPECT_EQ(cache.get_qout().size(), 0);
    EXPECT_EQ(*cache.get_qmain().begin(), "val1");

    // Step 3: Add a new element to Qin, causing "val1" to move to Qout
    cache.put(2, "val2");

    // Verify state after eviction
    EXPECT_EQ(cache.get_qin().size(), 1);
    EXPECT_EQ(cache.get_qmain().size(), 1);
    EXPECT_EQ(cache.get_qout().size(), 0);
    EXPECT_EQ(*cache.get_qin().begin(), "val2");
    EXPECT_EQ(*cache.get_qmain().begin(), "val1");

    // Step 4: Access "val1" again to move it back to Qmain
    cache.get(2);

    // Verify state after access
    EXPECT_EQ(cache.get_qin().size(), 0);
    EXPECT_EQ(cache.get_qmain().size(), 1);
    EXPECT_EQ(cache.get_qout().size(), 1);
    EXPECT_EQ(*cache.get_qmain().begin(), "val2");
    EXPECT_EQ(*cache.get_qout().begin(), "val1");
}

TEST(TwoQueue, PutExistingElementInQmain)
{
    TwoQueueConfig config = { 1, 1, 1 };
    TwoQueue<int, std::string> cache(config);

    // Add element to Qin and promote to Qmain
    cache.put(1, "val1");
    cache.get(1);

    // Verify it is in Qmain
    EXPECT_EQ(cache.get_qmain().size(), 1);
    EXPECT_EQ(*cache.get_qmain().begin(), "val1");

    // Add the same element again
    cache.put(1, "val1");

    // Verify state remains unchanged
    EXPECT_EQ(cache.get_qmain().size(), 1);
    EXPECT_EQ(*cache.get_qmain().begin(), "val1");
    EXPECT_EQ(cache.get_qin().size(), 0);
    EXPECT_EQ(cache.get_qout().size(), 0);
}

TEST(TwoQueue, EvictOldestFromQout)
{
    TwoQueueConfig config = { 1, 2, 1 };
    TwoQueue<int, std::string> cache(config);

    // Add elements to Qin and promote them to Qmain
    cache.put(1, "val1");
    cache.get(1);           // "val1" moves to Qmain
    cache.put(2, "val2");   // "val1" moves to Qout
    cache.put(3, "val3");   // "val2" moves to Qout

    // Verify state of Qout
    // in  : val3
    // out : val2
    // main: val1
    EXPECT_EQ(cache.get_qout().size(), 1);

    auto it = cache.get_qout().begin();
    EXPECT_EQ(*it++, "val2");

    cache.put(4, "val4");
    // in  : val4
    // out : val2, val3
    // main: val1

    EXPECT_EQ(cache.get_qout().size(), 2);
    EXPECT_EQ(cache.get_qin().size(), 1);
    EXPECT_EQ(cache.get_qmain().size(), 1);

    auto qout_it = cache.get_qout().begin();
    EXPECT_EQ(*qout_it++, "val2");
    EXPECT_EQ(*qout_it++, "val3");
    EXPECT_EQ(qout_it, cache.get_qout().end());   // Ensure no extra elements
}
