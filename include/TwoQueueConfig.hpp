#pragma once

#include <cstddef>

struct TwoQueueConfig
{
    std::size_t QinCapacity;
    std::size_t QoutCapacity;
    std::size_t QmainCapacity;
};
