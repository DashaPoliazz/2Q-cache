# **TwoQueue: A Cache Structure with Three Queues**

## **Overview**
[For more background on the principles behind this implementation, refer to the original TwoQueue replacement algorithm paper.](https://www.vldb.org/conf/1994/P439.PDF)
`TwoQueue` is a cache data structure implemented in C++. It divides elements into three different queues to optimize storage and quick access:

1. **Qin**: Holds recently added elements that have been used recently or are new to the cache.
2. **Qout**: Stores elements evicted from `Qin` to retain items that have been accessed recently but aren't currently in `Qin`.
3. **Qmain**: Keeps the most frequently accessed elements, prioritizing those with consistent usage.

This structure allows for efficient caching by balancing the access frequency of elements and avoiding cache pollution.

## **Features**
- **Efficient Management of Cache**: The cache maintains three queues to ensure elements are categorized based on usage frequency and recency.
- **Automatic Eviction**: When any of the queues reach their capacity, the least important elements are evicted and, if needed, moved between queues.
- **O(1) Access Time for Contains and Get**: The use of an unordered map (`lookup`) enables O(1) complexity for searching elements by key.

## **How it Works**
The `TwoQueue` cache structure implements a hybrid caching mechanism by maintaining three separate lists to balance the needs of recency and frequency of element usage. Here is a brief overview of how each part works:

- **Qin**: This queue stores recently added elements. When `Qin` reaches its capacity, the oldest element is moved to `Qout`.
- **Qout**: This queue holds elements evicted from `Qin`, providing an additional chance to move back into the main cache (`Qmain`) if they are accessed again.
- **Qmain**: Frequently accessed elements are moved into `Qmain`. If `Qmain` reaches its capacity, the oldest element is moved to `Qout`, giving less-used elements a chance to return if they become relevant again.

The data structure uses an `unordered_map` for tracking the position of elements in the queues, which allows for quick lookup, updates, and deletions.

## **Class and Methods**
### **Class Template**
The `TwoQueue` class is implemented as a templated structure:
```cpp
template <typename K, typename V>
struct TwoQueue {
    TwoQueue(TwoQueueConfig config);
    void put(const K& key, const V& value);
    std::optional<std::reference_wrapper<V>> get(const K& key);
    bool contains(const K& key);
    void erase(const K& key);
    // Additional getters for internal testing
};
```
### **Methods Description**
- **`put(const K& key, const V& value)`**: Adds a new key-value pair to the cache. If the key already exists, it updates its position to ensure it remains accessible.
- **`get(const K& key)`**: Retrieves the value associated with the given key, moving the element to `Qmain` if necessary. Returns `std::optional<std::reference_wrapper<V>>`, allowing for efficient reference handling.
- **`contains(const K& key)`**: Checks if the key exists in the cache.
- **`erase(const K& key)`**: Removes the key-value pair from the cache, handling its removal from whichever queue it belongs to.

## **TwoQueue Configuration**
The cache is configured using the `TwoQueueConfig` struct:
```cpp
struct TwoQueueConfig {
    size_t QinCapacity;
    size_t QoutCapacity;
    size_t QmainCapacity;
};
```
This configuration defines the maximum capacity of each queue (`Qin`, `Qout`, and `Qmain`). These values can be tuned depending on the desired behavior of the cache.

## **Usage Example**
Here is an example of how to use the `TwoQueue` cache:

```cpp
#include "two_queue.h"
#include <iostream>

int main() {
    TwoQueueConfig config{2, 2, 2}; // Qin, Qout, Qmain capacities
    TwoQueue<int, std::string> cache(config);

    cache.put(1, "Value1");
    cache.put(2, "Value2");
    cache.put(3, "Value3"); // This will move the oldest element from Qin to Qout

    if (auto value = cache.get(2)) {
        std::cout << "Key 2: " << value->get() << std::endl;
    }

    cache.erase(1); // Erase key 1

    if (!cache.contains(1)) {
        std::cout << "Key 1 has been erased." << std::endl;
    }

    return 0;
}
```

## **Strengths and Limitations**
### **Strengths**
- **Balanced Cache Management**: Combining multiple queues helps balance between recency and frequency, making it useful for a variety of caching scenarios.
- **Efficient Access**: The `lookup` map provides O(1) access to elements for operations like `get`, `contains`, and `erase`.

### **Limitations**
- **Memory Overhead**: The use of `std::list` and multiple unordered maps can lead to increased memory overhead, especially for large data sets.
- **Single-threaded Implementation**: The current implementation is not thread-safe and may need additional synchronization (`std::mutex`) for concurrent use.

## **Future Improvements**
- **Thread-Safety**: Introduce synchronization mechanisms to allow for concurrent access in multi-threaded applications.
- **Memory Optimization**: Consider using `std::deque` instead of `std::list` to reduce memory usage, as lists introduce extra pointers for each element.
- **Enhanced Replacement Policy**: Add more sophisticated eviction policies or make the eviction strategy more configurable.

## **Building and Running Tests**
To build and test this project, you can use **Google Test** as the testing framework. Make sure to include appropriate headers and link against the `gtest` library during compilation.

Here is an example build command:
```bash
g++ -std=c++17 -I<path_to_gtest_include> -o tests tests.cpp -L<path_to_gtest_lib> -lgtest -lgtest_main -pthread
./tests
```

