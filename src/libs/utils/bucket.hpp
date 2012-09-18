#ifndef __tp_utils_bucket_hpp__
#define __tp_utils_bucket_hpp__

#include <boost/array.hpp>
#include <boost/noncopyable.hpp>

#include <cstring>

namespace tp {
namespace utils {

    template <size_t S>
    class bucket : public boost::noncopyable
    {
    public:
        /*
         * Constructor without param
         */
        bucket();

        /*
         * Constructor with param
         * input:
         *      @index: an index defined by user
         */
        bucket(int index);

        /*
         * get the size of bucket
         */
        size_t size() const;

        /*
         * get user defined index, if not defined, -1 is returned
         */
        int index() const;

        /*
         * get the space that is left in this bucket
         */
        size_t space_left() const;

        /*
         * put the data into bucket
         * input:
         *      @data: data to be copied
         *      @size: the length of data
         * return:
         *      the size copied into bucket
         */
        size_t copy(const char* data, size_t size);

        /*
         * put a \0 terminated data into bucket
         * input:
         *      @data: \0 teminated data
         * return:
         *      the size copied into bucket
         */
        size_t copy(const char* data);

        /*
         * get the data from the bucket
         */
        const char* data();

        /*
         * reset the space_left to the size of bucket
         */
        void reset_space_left();

        /*
         * reset the space_left and reset content of bucket to 0 as well
         */
        void reset();

    private:
        boost::array<char, S+1> space_;
        size_t                  space_left_;
        int                     index_;
    };


    /*
     * Implementation starts from here
     */

    template <size_t S>
    inline
    bucket<S>::bucket()
            : space_left_(S)
            , index_(-1)
    {
        std::memset(space_.c_array(), 0, space_.size());
    }

    template <size_t S>
    inline
    bucket<S>::bucket(int index)
        : space_left_(S)
        , index_(index)
    {
        std::memset(space_.c_array(), 0, space_.size());
    }

    template<size_t S>
    inline size_t
    bucket<S>::size() const
    {
        return S;
    }

    template <size_t S>
    inline int
    bucket<S>::index() const
    {
        return index_;
    }

    template <size_t S>
    inline size_t
    bucket<S>::space_left() const
    {
        return space_left_;
    }

    template <size_t S>
    inline size_t
    bucket<S>::copy(const char* data, size_t size)
    {
        size_t offset = S - space_left_;
        size_t copy_size = size <= space_left_ ? size : space_left_;
        if (copy_size && data)
            std::memcpy(space_.c_array() + offset, data, copy_size);

        space_left_ -= copy_size;

        return copy_size;
    }

    template <size_t S>
    inline size_t
    bucket<S>::copy(const char* data)
    {
        if (space_left_ && data)
        {
            size_t size_copied = 0;
            size_t offset = S - space_left_;
            char* dest = space_.c_array() + offset;

            while (space_left_ && data[size_copied])
            {
                dest[size_copied] = data[size_copied];
                size_copied++;
                space_left_--;
            }
            return  size_copied;
        }

        return 0;
    }

    template <size_t S>
    inline const char*
    bucket<S>::data()
    {
        return space_.data();
    }

    template <size_t S>
    inline void
    bucket<S>::reset_space_left()
    {
        space_left_ = S;
    }

    template <size_t S>
    inline void
    bucket<S>::reset()
    {
        std::memset(space_.c_array(), 0, space_.size());
        reset_space_left();
    }

}}//tp::utils

#endif//__tp_utils_bucket_hpp__
