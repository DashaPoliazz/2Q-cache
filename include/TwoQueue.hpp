#pragma once

#include "./TwoQueueConfig.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <optional>
#include <unordered_map>

enum class QueueType
{
    Qin,
    Qout,
    Qmain
};

template <typename V>
struct LookupValue
{
    typename std::list<V>::iterator it;
    QueueType queue_type;

    LookupValue()
        : queue_type(QueueType::Qin)
    {
    }

    LookupValue(typename std::list<V>::iterator it, QueueType queue_type)
        : it(it)
        , queue_type(queue_type)
    {
    }
};

template <typename K, typename V>
struct TwoQueue
{
    TwoQueue(TwoQueueConfig config)
        : config(config)
    {
    }

    void put(const K& key, const V& value)
    {
        push(key, value);
    }

    std::optional<std::reference_wrapper<V>> get(const K& key)
    {
        auto it = lookup.find(key);
        /* nothing to move */
        if (it == lookup.end()) {
            return std::nullopt;
        }

        QueueType queue_type = it->second.queue_type;
        auto value_to_move = it->second.it;
        /* if the element is in 'Qmain' then it's either */
        if (queue_type == QueueType::Qmain) {
            /* on the last position */
            if (std::prev(Qmain.end()) == value_to_move) {
                return *value_to_move;
            }
            /* or it has to be moved on the last position */
            Qmain.splice(Qmain.end(), Qmain, value_to_move);
            it->second.it = std::prev(Qmain.end());
            return *value_to_move;
        }

        /* the element not in 'Qmain' and it should be moved in it */
        auto& current_queue = queue_type == QueueType::Qin ? Qin : Qout;
        current_queue.erase(value_to_move);

        push_Qmain(key, *value_to_move);

        return *std::prev(Qmain.end());
    }

    bool contains(const K& key)
    {
        return lookup.count(key);
    }

    void erase(const K& key)
    {
        auto it = lookup.find(key);
        if (it == lookup.end()) {
            return;
        }

        switch (it->second.queue_type) {
        case QueueType::Qin:
            Qin.erase(it->second.it);
            break;
        case QueueType::Qout:
            Qout.erase(it->second.it);
            break;
        case QueueType::Qmain:
            Qmain.erase(it->second.it);
            break;
        }

        reverse_lookup.erase(*it->second.it);
        lookup.erase(it);
    }

    const std::list<V>& get_qin() const
    {
        return Qin;
    }
    const std::list<V>& get_qout() const
    {
        return Qout;
    }
    const std::list<V>& get_qmain() const
    {
        return Qmain;
    }
    const std::unordered_map<K, LookupValue<V>> get_lookup() const
    {
        return lookup;
    }

private:
    void push(const K& key, const V& value)
    {
        /* if there is no such element it has to be just added in 'Qin' */
        auto it = lookup.find(key);
        bool exists = it != lookup.end();
        if (!exists) {
            /* evicting the oldest elements from 'Qin' & 'Qout' if needed */
            push_Qin(key, value);
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

        push_Qmain(key, value);
    }

    void push_Qin(const K& key, const V& value)
    {
        bool need_to_adjust_Qin = Qin.size() >= config.QinCapacity;
        if (need_to_adjust_Qin) {
            V oldest_Qin_element = Qin.front();
            K oldest_key = reverse_lookup[oldest_Qin_element];
            Qin.pop_front();
            push_Qout(oldest_key, oldest_Qin_element);
        }
        Qin.push_back(value);
        lookup[key] = { std::prev(Qin.end()), QueueType::Qin };
        reverse_lookup[value] = key;
    }

    void push_Qout(const K& key, const V& value)
    {
        bool need_to_adjust_Qout = Qout.size() >= config.QoutCapacity;
        if (need_to_adjust_Qout) {
            V oldest_Qout_element = Qout.front();
            K oldest_key = reverse_lookup[oldest_Qout_element];
            Qout.pop_front();
            lookup.erase(oldest_key);
            reverse_lookup.erase(oldest_Qout_element);
        }
        Qout.push_back(value);
        lookup[key] = { std::prev(Qout.end()), QueueType::Qout };
        reverse_lookup[value] = key;
    }

    void push_Qmain(const K& key, const V& value)
    {
        /* adjusting Qmain */
        if (Qmain.size() >= config.QmainCapacity) {
            V oldest_Qmain_element = Qmain.front();
            K oldest_key = reverse_lookup[oldest_Qmain_element];
            Qmain.pop_front();
            // lookup.erase(oldest_key);
            // reverse_lookup.erase(oldest_Qmain_element);
            push_Qout(oldest_key, oldest_Qmain_element);
        }
        /* adding to the 'Qmain' */
        Qmain.push_back(value);
        lookup[key] = { std::prev(Qmain.end()), QueueType::Qmain };
        reverse_lookup[value] = key;
    }

    TwoQueueConfig config;
    std::list<V> Qin;
    std::list<V> Qout;
    std::list<V> Qmain;
    std::unordered_map<K, LookupValue<V>> lookup;
    std::unordered_map<V, K> reverse_lookup;
};
