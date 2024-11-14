#ifndef CACHE_QUEUE
#define CACHE_QUEUE

#include <list>

template <typename T>
struct CacheQueue : public std::list<T>
{
    CacheQueue(std::size_t capacity)
        : capacity(capacity)
    {
    }

    void push_back(const T& value)
    {
        if (std::list<T>::size() >= capacity) {
            std::list<T>::pop_front();
        }

        std::list<T>::push_back(value);
    }

private:
    std::size_t capacity;
};

#endif
