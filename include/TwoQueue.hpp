#ifndef TWO_QUEUE
#define TWO_QUEUE

#include <list>
#include <unordered_map>

template <typename T>
struct TwoQueue
{
    /* constructor */
    TwoQueue(std::size_t capacity)
        : capacity(capacity)
    {
    }

    void push(const T& value)
    {
    }

    void pop()
    {
    }

private:
    std::size_t capacity;
    std::list<T> Qin;
    std::list<T> Qout;
    std::list<T> Qmain;
    std::unordered_map<const T&, typename std::list<T>::iterator> lookup;
};

#endif
