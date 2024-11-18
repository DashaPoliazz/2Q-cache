#include "../include/TwoQueue.hpp"
#include <gtest/gtest.h>

TEST(TwoQueueTest, EraseElementFromQin)
{
    TwoQueueConfig config{ 3, 3, 3 };
    TwoQueue<int, std::string> cache(config);

    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");

    ASSERT_EQ(cache.get_qin().size(), 3);

    cache.erase(2);

    EXPECT_EQ(cache.get_qin().size(), 2);
    EXPECT_EQ(cache.contains(2), false);
}

TEST(TwoQueueTest, EraseElementFromQout)
{
    TwoQueueConfig config{ 2, 2, 2 };
    TwoQueue<int, std::string> cache(config);

    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");
    cache.put(4, "four");

    ASSERT_EQ(cache.get_qout().size(), 2);
    ASSERT_EQ(cache.contains(1), true);
    ASSERT_EQ(cache.contains(2), true);

    cache.erase(1);

    EXPECT_EQ(cache.get_qout().size(), 1);
    EXPECT_EQ(cache.contains(1), false);
}

TEST(TwoQueueTest, EraseElementFromQmain)
{
    TwoQueueConfig config{ 2, 2, 2 };
    TwoQueue<int, std::string> cache(config);

    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");
    cache.put(4, "four");

    cache.get(3);

    ASSERT_EQ(cache.get_qmain().size(), 1);
    ASSERT_EQ(cache.contains(3), true);

    // Удаляем элемент из Qmain
    cache.erase(3);

    // Проверяем, что элемент удален
    EXPECT_EQ(cache.get_qmain().size(), 0);
    EXPECT_EQ(cache.contains(3), false);
}

TEST(TwoQueueTest, EraseNonExistentElement)
{
    TwoQueueConfig config{ 3, 3, 3 };
    TwoQueue<int, std::string> cache(config);

    cache.put(1, "one");
    cache.put(2, "two");

    ASSERT_EQ(cache.contains(3), false);

    // Удаляем несуществующий элемент
    cache.erase(3);

    // Проверяем, что ничего не изменилось
    EXPECT_EQ(cache.get_qin().size(), 2);
    EXPECT_EQ(cache.contains(1), true);
    EXPECT_EQ(cache.contains(2), true);
}

TEST(TwoQueueTest, EraseAndAddNewElement)
{
    TwoQueueConfig config{ 2, 2, 2 };
    TwoQueue<int, std::string> cache(config);

    cache.put(1, "one");
    cache.put(2, "two");

    ASSERT_EQ(cache.get_qin().size(), 2);

    // Удаляем элемент и добавляем новый
    cache.erase(1);
    cache.put(3, "three");

    // Проверяем, что новый элемент добавлен, а старый удален
    EXPECT_EQ(cache.get_qin().size(), 2);
    EXPECT_EQ(cache.contains(1), false);
    EXPECT_EQ(cache.contains(3), true);
}
