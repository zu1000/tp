#ifndef __tp_utils_ring_buffer_hpp__
#define __tp_utils_ring_buffer_hpp__

#include <boost/array.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/static_assert.hpp>
#include <boost/thread/mutex.hpp>

namespace tp {
namespace utils {

    /*
     * TODO: Make ring_buffer capacity dynamical
     *       This is only safe for one producer and one consumer
     */
    template <typename B, size_t S>
    class ring_buffer : public boost::noncopyable
    {
    public:
        typedef B                       bucket_type;
        typedef boost::scoped_ptr<B>    bucket_ptr_type;

        BOOST_STATIC_ASSERT(S > 2);

        ring_buffer();

        /*
         * Put message into the ring buffer
         * input:
         *      @msg: \0 ended char buffer
         * return:
         *      the size of message that is put into the ring buffer
         */
        size_t put(const char* msg);

        /*
         * Put message into the ring buffer
         * input:
         *     @msg:  char buffer
         *     @size: size of char buffer
         * return:
         *      the size of message that is put into the ring buffer
         */
        size_t put(const char* msg, size_t size);

        /*
         * cosume the buffer that is read
         * input:
         *      no input
         * return:
         *      the next readable bucket ptr, the buffer is \0 ended.
         *      a null ptr is returned, if there is no valid bucket
         */
        const bucket_ptr_type& get();

        /*
         * force to skip to the next buffer.
         * input:
         *      none
         * return:
         *      true, if we can flush current buffer
         *      false, there no free bucket and current buffer cannot be flush
         */
        bool flush();

        /*
         * reset the ring_buffer. light weight.
         */
        void reset();

        /*
         * reset the ring_bufer, clean up all buckets. heavy
         */
        void reset_all();

    private:
        int bucket_idx_to_get_;
        boost::array<bucket_ptr_type, S>  buckets_;
        bucket_ptr_type                   bucket_;
        boost::mutex                      mutex_;
    };

    /*
     * Implementation starts from here
     */
    template <typename B, size_t S>
    inline
    ring_buffer<B, S>::ring_buffer()
        : bucket_idx_to_get_(0)
        , bucket_(NULL)
    {
        for (size_t i = 0; i < S; ++i)
        {
            buckets_[i].reset(new B(i));
        }

        buckets_[0].swap(bucket_);
    }

    template <typename B, size_t S>
    inline size_t
    ring_buffer<B, S>::put(const char* msg)
    {
        if (!bucket_->space_left() && !flush())
            return 0;

        return bucket_->copy(msg);
    }

    template <typename B, size_t S>
    inline size_t
    ring_buffer<B, S>::put(const char* msg, size_t size)
    {
        size_t size_to_copy = size;
        while (size_to_copy > 0)
        {
            size_t offset = size - size_to_copy;

            if (!bucket_->space_left() && !flush())
                return offset;

            size_to_copy -= bucket_->copy(msg + offset, size_to_copy);
        }

        return size;
    }

    template <typename B, size_t S>
    inline const typename ring_buffer<B, S>::bucket_ptr_type&
    ring_buffer<B, S>::get()
    {
        bucket_ptr_type& ptr = buckets_[bucket_idx_to_get_];

        // this should be atomic
        if (ptr)
        {
            boost::mutex::scoped_lock l(mutex_);
            bucket_idx_to_get_ = (bucket_idx_to_get_ + 1) % S;
        }

        return ptr;
    }

    template <typename B, size_t S>
    inline bool
    ring_buffer<B, S>::flush()
    {
        int next_available = (bucket_->index() + 1) % S;

        // this should be atomic
        do
        {
            boost::mutex::scoped_lock l(mutex_);
            if (next_available == bucket_idx_to_get_) return false;
        }
        while(0);

        buckets_[bucket_->index()].swap(bucket_);
        buckets_[next_available].swap(bucket_);

        return true;
    }

    template <typename B, size_t S>
    inline void
    ring_buffer<B, S>::reset()
    {
        bucket_idx_to_get_ = bucket_->index();
        bucket_->reset_space_left();
    }

    template <typename B, size_t S>
    inline void
    ring_buffer<B, S>::reset_all()
    {
        bucket_idx_to_get_ = bucket_->index();
        bucket_->reset();

        for (size_t i = 0; i < S; ++i)
            if (buckets_[i])
                buckets_[i]->reset();
    }

}} //tp::utils

#endif//__tp_utils_ring_buffer_hpp__
