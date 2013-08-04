#ifndef __blade_fix_h__
#define __blade_fix_h__

#include <string>
#include <cstring>
#include <vector>
#include <stdint.h>
#include <assert.h>
#include <math.h>

#include <iostream>

namespace blade { namespace fix {

const uint64_t b64_sign_mask = uint64_t(1) << 63;

inline uint64_t i64abs(int64_t i)
{
    if (i < 0) return (~i + 1);
    return i;
}

template <size_t N>
char* i_to_str(int64_t v, char(&b)[N])
{
    assert(N>=31);
    size_t sign_offset = (1 ^ (v >> 63)) & 0x1;
    uint64_t i = i64abs(v);
    int n = N; b[n--] = 0;
    do
    {
        b[n--] = i%10 + '0';
        i /= 10;
    }while(i && n >= 0);

    b[n] = '-';

    return &b[n+sign_offset];
}

template <size_t N>
char* ui_to_str(uint64_t u, char(&b)[N])
{
    assert(N>=31);
    int n = 30; b[n--] = 0;
    do
    {
        b[n--] = u%10 + '0';
        u /= 10;
    }while(u && n >= -1);

    return &b[n+1];
}

template <size_t N>
char* db_to_str(double v, size_t I, size_t F, char(&b)[N])
{
    assert(I+F+1 < N);

    double factor = pow(10, F);
    uint64_t n = i64abs(llround(v*factor));

    char* d = b + N - 1;

    for(size_t i = F; i > 0; --i, --d)
    {
        *d = n%10 + '0';
        n /= 10;
    }

    *d = '.'; d--;

    for(size_t i = I; i > 0; --i, --d)
    {
        *d = n%10 + '0';
        n /= 10;
        if (!n) {--d; break;}
    }

    if (v < 0)
    {
        *d = '-';
        return d;
    }
    else
        return d+1;
}

void i_to_str(int64_t i, std::string& d)
{
    char b[31] = {0};
    d = i_to_str(i, b);
}

void ui_to_str(uint64_t u, std::string& d)
{
    char b[31] = {0};
    d = ui_to_str(u, b);
}

class field
{
public:
    field(unsigned tag, size_t tag_offset, size_t val_size)
    {
        ui_to_str(tag, tag_);
        tag_ += '=';
        tag_offset_ = tag_offset;
        tag_size_   = tag_.length();
        val_offset_ = tag_offset_ + tag_size_;
        val_size_   = val_size;
        prev_tag_   = -1;
        next_tag_   = -1;
    }

    size_t val_size() const
    {
        return val_size_;
    }

    size_t tag_size() const
    {
        return tag_size_;
    }

    size_t space() const
    {
        return tag_size_ + val_size_ + 1;
    }

    void resize_value(size_t val_size)
    {
        val_size_ = val_size;
    }

    void offset_delta(int delta)
    {
        tag_offset_ += delta;
        val_offset_ += delta;
    }

    const std::string& tag() const
    {
        return tag_;
    }

    void prev_tag(unsigned tag)
    {
        prev_tag_ = tag;
    }

    int64_t prev_tag() const
    {
        return prev_tag_;
    }

    void next_tag(unsigned tag)
    {
        next_tag_ = tag;
    }

    int64_t next_tag() const
    {
        return next_tag_;
    }

    size_t tag_offset() const
    {
        return tag_offset_;
    }

    size_t val_offset() const
    {
        return val_offset_;
    }

private:
    field()
        : tag_offset_()
        , val_offset_()
        , tag_size_()
        , val_size_()
    {}

    friend class std::vector<field>;
    std::string tag_;
    size_t  tag_offset_;
    size_t  val_offset_;
    size_t  tag_size_;
    size_t  val_size_;
    int64_t prev_tag_;
    int64_t next_tag_;
};

typedef std::vector<field> fields;

// This builder is supposed to have static size or size is rarely changed
template<size_t buff_size>
class simple_builder
{
public:
    simple_builder()
        : space_()
        , chksum_()
        , fst_tag_(-1)
        , lst_tag_(-1)
        , finalized_(false)
    {}

    bool push_tag(unsigned tag, size_t size)
    {
        return push_tag(tag, size, false);
    }

    void finalize()
    {
        if (finalized_)
            return;

        // push the tailer
        push_tag(10, 3, true);

        int64_t tag = fst_tag_;

        do
        {
            const field& f = fields_[tag];
            std::memcpy(buff_+f.tag_offset(), f.tag().c_str(), f.tag_size());
            std::memset(buff_+f.val_offset(), '0', f.val_size());
            buff_[f.val_offset()+f.val_size()] = 1; //SOH
            tag = f.next_tag();
        }while(tag >= 0);

        for (size_t i = 0; i < (space_ - fields_[10].space()); ++i)
            chksum_ += buff_[i];

        finalized_ = true;
    }

    void resize_val_size(unsigned tag, size_t sz, char pad = '0')
    {
        if (!finalized_) return;

        assert(sz);

        field& f = fields_[tag];

        assert(f.val_size());

        int changed_size = sz - f.val_size();

        if (0 == changed_size) return;

        assert(space_ + changed_size < sizeof(buff_));

        char* s = buff_ + f.val_offset();
        char* d = buff_ + f.val_offset();
        int chksum_delta = 0;

        if (changed_size > 0)
        {
            d += changed_size;
            chksum_delta = pad * changed_size;
        }
        else
        {
            s += changed_size;
            for (int i = 0; i < -changed_size; ++i)
                chksum_delta -= d[i];
        }

        std::memmove(d, s, (buff_+space_) - s);

        if (changed_size > 0)
            for (int i = 0; i < changed_size; ++i)
                d[0] = pad;

        space_  += changed_size;
        chksum_ += chksum_delta;
    }

    void set_value(unsigned tag, const std::string& v, char pad = '0')
    {
        std::cout << v << std::endl;
        set_value(tag, v.c_str(), v.size(), pad);
    }

    template <size_t N>
    void set_value(unsigned tag, const char(&v)[N], char pad = '0')
    {
        int n = N-1;
        for (; n >= 0; --n)
            if (v[n]) break;

        set_value(tag, v, n+1, pad);
    }

    void set_value(unsigned tag, const char* v, size_t N, char pad = '0')
    {
        if (!finalized_) return;

        assert(tag < fields_.size());

        field& f = fields_[tag];

        assert(N <= f.val_size());

        char *d = buff_ + f.val_offset() + f.val_size() - 1;

        int chksum_delta = 0;
        for (int i = N-1; i >= 0; --i)
        {
            int delta = v[i] - *d;
            *d = v[i];
            chksum_delta += delta;
            d--;
        }

        if (pad)
            for (size_t i = 0; i < f.val_size() - N; ++i)
            {
                int delta = pad - *d;
                *d = pad;
                chksum_delta += delta;
                d--;
            }

        chksum_ += chksum_delta;
    }

    template<size_t F>
    void set_value(unsigned tag, double v, bool pad=false)
    {
        if (!finalized_) return;

        assert(tag < fields_.size());

        field& f = fields_[tag];

        size_t I = f.val_size() - (F+1);

        static double factor = pow(10, F);
        uint64_t n = i64abs(llround(v*factor));

        char* d = buff_ + f.val_offset() + f.val_size() - 1;

        int chksum_delta = 0;
        for(size_t i = F; i > 0; --i)
        {
            int delta = n%10 - ((*d) - '0');
            *d += delta;
            chksum_delta += delta;
            n /= 10; d--;
        }
  
        chksum_delta += '.' - *d;
        *d = '.'; d--;

        for(size_t i = I; i > 0; --i)
        {
            int delta = n%10 - ((*d) - '0');
            *d += delta;
            chksum_delta += delta;
            n /= 10; d--;
            if (0 == v && !pad)
                break;
        }

        if (v < 0)
        {
           d = buff_+f.val_offset();
           chksum_delta += '-' - d[0];
           d[0] = '-';
        }
        else
        {
           d = buff_+f.val_offset();
           chksum_delta += '0' - d[0];
           d[0] = '0';
        }

        chksum_ += chksum_delta;
    }

    void set_value(unsigned tag, uint64_t v, bool pad=false)
    {
        if (!finalized_) return;

        assert(tag < fields_.size());

        field& f = fields_[tag];
        assert(f.val_size());

        int chksum_delta = 0;

        char* d = buff_ + f.val_offset() + f.val_size() - 1;

        while(d >= buff_ + f.val_offset())
        {
            int delta = (v%10 - (*d - '0'));
            *d += delta;
            chksum_delta += delta;
            v /= 10; d--;

            if (0 == v && !pad)
            {
                d = buff_ + f.val_offset();
                break;
            }
        }

        assert(!v);

        chksum_ += chksum_delta;
    }

    void set_value(unsigned tag, int64_t v, bool pad=false)
    {
        if (!finalized_) return;

        assert(tag < fields_.size());

        field& f = fields_[tag];
        assert(f.val_size());

        int chksum_delta = 0;
        bool neg = v < 0;
        v = i64abs(v);

        char* d = buff_ + f.val_offset() + f.val_size() - 1;

        while(d > buff_ + f.val_offset())
        {
            int delta = (v%10 - (*d - '0'));
            *d += delta;
            chksum_delta += delta;
            v /= 10; d--;

            if (0 == v && !pad)
            {
                d = buff_ + f.val_offset();
                break;
            }
        }

        if (neg)
        {
            int delta = '-' - *d; 
            *d = '-';
            chksum_delta += delta;
        }
        else
        {
            int delta = '0' - *d; 
            *d = '0';
            chksum_delta += delta;
        }

        assert(!v);

        chksum_ += chksum_delta;
    }

    size_t chksum() const
    {
        return chksum_;
    }

    size_t space() const
    {
        return space_;
    }

    template <typename S>
    bool send(S& s)
    {
        set_tailer();
        s.send(buff_, space_);
    }

    size_t get_length_from(unsigned tag)
    {
        if (!finalized_) return 0;

        assert(tag < fields_.size());

        const field& f = fields_[tag];
        assert(f.val_size());

        return space_ - f.tag_offset();
    }

    size_t copy_to(char* d, size_t sz, bool include_tailer=true)
    {
        size_t size_to_copy = space_;
        if (include_tailer)
            set_tailer();
        else
            size_to_copy -= fields_[10].space();

        assert(sz > size_to_copy);
        memcpy(d, buff_, size_to_copy);
        return size_to_copy;
    }

private:
    void set_tailer()
    {
        // have to save and recover the chksum_, since set_value will change it
        size_t chksum = chksum_;
        set_value(10, uint64_t(chksum_ & 0xff));
        chksum_ = chksum;
    }

    bool push_tag(unsigned tag, size_t size, bool allow_tag10)
    {
        // Tag 10 is ignored since it is padded automatically in finalize()
        if (tag == 10 && !allow_tag10)
            return false;

        // after finalization, no more push
        if (finalized_)
            return false;

        if (!size)
            return false;

        // remember the first tag
        if (fst_tag_ == -1)
            fst_tag_ = tag;

        // prepare field
        field f(tag, space_, size);

        // resize if the fields vector is not big enough
        // expensive, but initialization should only done once
        if (fields_.size() <= tag)
            fields_.resize(tag+1);
        else
        if (fields_[tag].val_size()) // already exists
            return false;

        fields_[tag] = f;

        // set the last tag
        if (lst_tag_ >= 0)
        {
            fields_[lst_tag_].next_tag(tag);
            fields_[tag].prev_tag(lst_tag_);
        }

        lst_tag_ = tag;
        space_   += fields_[tag].space();

        return true;
    }

protected:
    size_t  space_;
    size_t  chksum_;
    int64_t fst_tag_;
    int64_t lst_tag_;
    fields  fields_;
    bool    finalized_;
    char    buff_[buff_size];
};

template<size_t buff_size>
class extensible_builder : public simple_builder<buff_size>
{
    typedef simple_builder<buff_size> base_type;
public:
    extensible_builder()
        : extended_size_()
        , extended_chksum_()
    {}

    void reset()
    {
        extended_size_ = 0;
        extended_chksum_ = 0;
    }

    template<typename builder_type>
    void copy_from(const builder_type& sb)
    {
        if (!this->finalized_)
            return;
        size_t size_to_copy = sb.space() - this->fields_[10].space();
        size_t size_left = buff_size - (this->space()+extended_size_);
        // TODO: Check the size
        assert(size_to_copy < size_left);
        char* d = this->buff_ + (this->space() - this->field_[10]) + extended_size_;
        sb.copy_to(d, size_left, false);
        extended_size_ += size_to_copy;
        extended_chksum_ += sb.chksum();
    }

    template <typename S>
    bool send(S& s)
    {
        assert(this->finalized_);

        // prepare tailer
        size_t size_to_send = this->space()+extended_size_;
        uint64_t chksum = (this->chksum()+extended_chksum_)&0xff;
        char* tailer_offset = this->buff_ + size_to_send - this->fields_[10].space();

        char tailer[7] = {'1', '0', '=', '0', '0', '0', '0'};
        i_to_str(chksum, tailer);

        memcpy(tailer_offset, tailer, 7);

        // send buffer
        s.send(this->buff_, size_to_send);
    }

    size_t extended_size() const
    {
        return extended_size_;
    }

    size_t extended_chksum() const
    {
        return extended_chksum_;
    }

private:
    size_t extended_size_;
    size_t extended_chksum_;
};

class stream_builder
{
public:
    stream_builder(char* buff_, size_t size)
        : pos_(buff_) 
        , beg_(buff_)
        , end_(buff_+size)
        , finalized_(false)
    {}

    bool push_tag(unsigned tag, size_t size)
    {
        // Tag 10 is ignored since it is padded automatically in finalize()
        if (tag == 10)
            return false;

        // after finalization, no more push
        if (finalized_)
            return false;

        // prepare field
        field f(tag, 0, size);

        if (fields_.size() <= tag)
            fields_.resize(tag+1);
        fields_[tag] = f;

        return true;
    }

    void finalize()
    {
        finalized_ = true;
    }

    void reset()
    {
        finalized_ = false;
        fields_.clear();
        pos_ = beg_;
    }

    void set_value(unsigned tag, const std::string& str)
    {
        set_value(tag, str.c_str(), str.length());
    }

    void set_value(unsigned tag, const char* str, size_t sz)
    {
        assert(tag < fields_.size());

        field& f = fields_[tag];

        assert(!f.tag().empty());

        std::memcpy(pos_, f.tag().c_str(), f.tag_size());

        std::memcpy(pos_+f.tag_size(), str, sz);

        add_up_chksum(f.tag_size() + sz);

        // SOH
        *pos_ = 1;
        chksum_++;
    }

    template<size_t I, size_t F>
    void set_value(unsigned tag, double v)
    {
        assert(tag < fields_.size());

        field& f = fields_[tag];

        assert(!f.tag().empty());

        std::memcpy(pos_, f.tag().c_str(), f.tag_size());

        char str[I+F+2];
        char *s = db_to_str<I+F+2>(v, I, F, str);

        int size_to_copy = str + sizeof(str) - s;

        std::memcpy(pos_+f.tag_size(), s, size_to_copy);

        add_up_chksum(f.tag_size() + size_to_copy);

        // SOH
        *pos_ = 1;
        chksum_++;
    }

    void set_value(unsigned tag, int64_t v)
    {
        assert(tag < fields_.size());

        field& f = fields_[tag];

        assert(!f.tag().empty());

        std::memcpy(pos_, f.tag().c_str(), f.tag_size());

        char str[31];
        char * s = i_to_str(v, str);

        int size_to_copy = str + sizeof(str) - s;

        std::memcpy(pos_+f.tag_size(), s, size_to_copy);

        add_up_chksum(f.tag_size() + size_to_copy);

         // SOH
        *pos_ = 1;
        chksum_++;           
    }

    template <typename S>
    bool send(S& s)
    {
        set_tailer();
        s.send(beg_, pos_-beg_);
    }

private:
    void set_tailer()
    {
        memcpy(pos_, "10=", 3);
        
        char str[31];
        memset(str+28, '0', 3);
        ui_to_str(chksum_&0xff, str);
        memcpy(pos_+3, str+28, 3);

        pos_[6] = 1;
        pos_ += 7;
    }

    // This function will really move the buffer pointer
    void add_up_chksum(size_t sz)
    {
        for (size_t i = 0; i < sz; ++i, ++pos_)
            chksum_ += *pos_;
    }

private:
    char*   pos_;
    char*   beg_;
    char*   end_;
    size_t  chksum_;
    fields  fields_;
    bool    finalized_;
};

}} // blade::fix
#endif//__blade_fix_h__
