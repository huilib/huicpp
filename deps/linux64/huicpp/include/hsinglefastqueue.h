/**********************************************************
 * Created: Joseph Feb-07-2022
 * Description: 
 * 		Wrapper for fast queue that for one producer and 
 *      one consumer without locks.
 **********************************************************/

#ifndef __H_HUICPP_SINGLE_FAST_QUEUE_H__
#define __H_HUICPP_SINGLE_FAST_QUEUE_H__

#include "huicpp.h"
#include <internal/constant_in.h>

#include <atomic>
#include <type_traits>
#include <cassert>

namespace HUICPP {

template <class _Ty> 
class HSingleFastQueue {
public:
    using value_type = _Ty;
    using atomic_index_t = std::atomic<HUN>;
    using size_type = HUN;
    using pointer_type = value_type*;
    using data_pointer = value_type* const;

public:
    explicit HSingleFastQueue(size_type _size)
        : m_size(_size),
        m_data(static_cast<pointer_type>(malloc(sizeof(value_type) * m_size))),
        m_read_index(0),
        m_write_index(0) {
        // size must be >= 2.
        /*
            Also, note that the number of usable slots in queue at any
            given time is actually (size - 1).
        */
        assert(m_size >= 2);
        assert(m_data != nullptr);
    }

    ~ HSingleFastQueue() {
        // We need to destruct anything that may still exist in our queue.
        if (not std::is_trivially_destructible<value_type>::value) {
            HSIZE read_index = m_read_index;
            HSIZE write_index = m_write_index;
            while(read_index != write_index) {
                m_data[read_index].~value_type();
                if (++read_index == m_size) {
                    read_index = 0;
                }
            }
        }
        free(m_data);
    }

    HSingleFastQueue(const HSingleFastQueue&) = delete;

    HSingleFastQueue& operator= (const HSingleFastQueue&) = delete;

public:
    template<class... Args>
    bool Push(Args&& ... valueArgs) {
        auto const current_push_index = m_write_index.load(std::memory_order_relaxed);
        auto next_pos = current_push_index + 1;
        if (next_pos == m_size) {
            next_pos = 0;
        }
        if (next_pos != m_read_index.load(std::memory_order_acquire)) {
            new (&m_data[next_pos]) value_type(std::forward<Args>(valueArgs)...);
            m_write_index.store(next_pos, std::memory_order_release);
            return true;
        }        
        // queue is full.
        return false;
    }

    // move (or copy) the value at the front of the queue to given variable.
    bool Get(value_type& value) {
        auto const current_get_index = m_read_index.load(std::memory_order_relaxed);
        if (current_get_index == m_write_index.load(std::memory_order_acquire)) {
            // queue is empty
            return false;
        }
        auto next_pos = current_get_index + 1;
        if (next_pos == m_size) {
            next_pos = 0;
        }
        value = std::move(m_data[next_pos]);
        m_data[current_get_index].~value_type();
        m_read_index.store(next_pos, std::memory_order_release);
        return true;
    }

    // pointer to the value at the front of the queue (for use in-place) 
    // or nullptr if empty
    pointer_type FrontPtr() {
        auto const current_get_pos = m_read_index.load(std::memory_order_relaxed);
        if (current_get_pos == m_write_index.load(std::memory_order_acquire)) {
            // queue is empty
            return nullptr;
        }
        return &m_data[current_get_pos];
    }

    // queue must not be empty
    void popFront() {
        auto const current_get_pos = m_read_index.load(std::memory_order_relaxed);
        assert(current_get_pos != m_write_index.load(std::memory_order_acquire));
        auto next_pos = current_get_pos + 1;
        if (next_pos == m_size) {
            next_pos = 0;
        }
        m_data[current_get_pos].~value_type();
        m_read_index.store(next_pos, std::memory_order_release);
    }

    bool isEmpty() const {
        return m_read_index.load(std::memory_order_acquire) == 
            m_write_index.load(std::memory_order_acquire);
    }

    bool isFull() const {
        auto next_put_pos = m_write_index.load(std::memory_order_acquire) + 1;
        if (next_put_pos == m_size) {
            next_put_pos = 0;
        }
        if (next_put_pos != m_read_index.load(std::memory_order_acquire)) {
            return false;
        }
        // queue is full.
        return true;
    }

    /*
        If called by consumer, then true size is may be more (because producer may
            be adding itmes concurrently).
        If called by producer, then true size is may be less (because consumer may
            be removing items concurrently).
        It is undefined to call this from any other thread.
    */
    HSIZE GuestSize() const {
        HN res = m_write_index.load(std::memory_order_acquire) - 
            m_read_index.load(std::memory_order_acquire);
        if (res < 0) {
            res += m_size;
        }
        return res;
    }

    // the maximum number of items in the queue.
    HSIZE Capacity () const { return m_size - 1; }

private:
    HCH memory_pad_begin[HARDWARE_INTERFERENCE_SIZE];
    const size_type m_size;
    data_pointer m_data;
    alignas(HARDWARE_INTERFERENCE_SIZE) atomic_index_t m_read_index;
    alignas(HARDWARE_INTERFERENCE_SIZE) atomic_index_t m_write_index;
    HCH memory_pad_end[HARDWARE_INTERFERENCE_SIZE - sizeof(atomic_index_t)];
};

}

#endif //__H_HUICPP_SINGLE_FAST_QUEUE_H__

