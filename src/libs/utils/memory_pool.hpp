#ifndef __tp_utils_memory_pool_hpp__
#define __tp_utils_memory_pool_hpp__

#include <boost/static_assert.hpp>

namespace tp {
namespace utils {

    template<std::size_t capacity, std::size_t max_bulk_size>
    class memory_pool
    {

        BOOST_STATIC_ASSERT(capacity);
        BOOST_STATIC_ASSERT(max_bulk_size);
        BOOST_STATIC_ASSERT(capacity > max_buik_size);
        BOOST_STATIC_ASSERT(0 == (capacity%sizeof(size_t)));
        BOOST_STATIC_ASSERT(0 == (max_bulk_size%sizeof(size_t)));

        memory_pool()
            : alignment_(sizeof(size_t))
            , capacity_(capacity)
            , max_bulk_size_(max_bulk_size)
        {}

    private:
        const size_t alignment_;
        const size_t capacity_;
        const size_t max_bulk_size_;
    };

}} // tp::utils

#endif//__tp_utils_memory_pool_hpp__
