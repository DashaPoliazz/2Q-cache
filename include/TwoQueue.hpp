#ifndef TWO_QUEUE
#define TWO_QUEUE

#include <list>
#include <unordered_map>

enum class QueueType
{
    Qin,
    Qout,
    Qmain
};

template <typename T>
struct LookupValue
{
    typename std::list<T>::iterator it;
    QueueType queue_type;

    LookupValue()
        : queue_type(QueueType::Qin)
    {
    }   // Default QueueType (optional)

    LookupValue(typename std::list<T>::iterator it, QueueType queue_type)
        : it(it)
        , queue_type(queue_type)
    {
    }
};

struct TwoQueueConfig
{
    std::size_t QinCapacity;
    std::size_t QoutCapacity;
    std::size_t QmainCapacity;
};

template <typename T>
struct TwoQueue
{
    /* constructor */
    TwoQueue(TwoQueueConfig config)
        : config(config)
    {
    }

    void push(const T& value)
    {
        /* if there is no such element it has to be just added in 'Qin' */
        auto it = lookup.find(value);
        bool exists = it != lookup.end();
        if (!exists) {
            /* evicting the oldest elements from 'Qin' & 'Qout' if needed */
            bool need_to_adjust_Qin = Qin.size() >= config.QinCapacity;
            /* it has to be moved to 'Qout' */
            if (need_to_adjust_Qin) {
                /* adjusting 'Qout' if it's needed */
                bool need_to_adjust_Qout = Qout.size() >= config.QoutCapacity;
                if (need_to_adjust_Qout) {
                    T oldest_Qout_element = Qout.front();
                    Qout.pop_front();
                    lookup.erase(oldest_Qout_element);
                }
                /* evicting element from 'Qin' and moving it to adjusted 'Qout' */
                T oldest_Qin_element = Qin.front();
                Qin.pop_front();
                Qout.push_back(oldest_Qin_element);
                lookup[oldest_Qin_element] = { std::prev(Qout.end()), QueueType::Qout };
            }
            /* adding element in the 'Qin' */
            Qin.push_back(value);
            lookup[value] = { std::prev(Qin.end()), QueueType::Qin };
            return;
        }

        /* otherwise we have to move it to 'Qmain' */
        QueueType queue_type = it->second.queue_type;
        /* aldredy in 'Qmain' */
        if (queue_type == QueueType::Qmain) {
            return;
        }
        /* removing element either from 'Qin' or 'Qout' and inserting in 'Qmain' */
        auto iter_to_erase = it->second.it;
        if (queue_type == QueueType::Qin) {
            Qin.erase(iter_to_erase);
        } else {
            Qout.erase(iter_to_erase);
        }

        /* adjusting Qmain */
        if (Qmain.size() >= config.QmainCapacity) {
            T oldest_Qmain_element = Qmain.front();
            Qmain.pop_front();
            lookup.erase(oldest_Qmain_element);
        }
        /* adding to the 'Qmain' */
        Qmain.push_back(value);
        lookup[value] = { std::prev(Qmain.end()), QueueType::Qmain };
    }

    void pop()
    {
    }

    /* for testing purposes */
    const std::list<T>& get_qin() const
    {
        return Qin;
    }
    const std::list<T>& get_qout() const
    {
        return Qout;
    }
    const std::list<T>& get_qmain() const
    {
        return Qmain;
    }
    const std::unordered_map<T, LookupValue<T>> get_lookup() const
    {
        return lookup;
    }

private:
    TwoQueueConfig config;
    std::list<T> Qin;
    std::list<T> Qout;
    std::list<T> Qmain;
    std::unordered_map<T, LookupValue<T>> lookup;
};

#endif
