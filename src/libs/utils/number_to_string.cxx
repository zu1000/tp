#include "number_to_string.hpp"

namespace tp {
namespace utils {

    static const uint64_t pow10[] = 
    {0, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

    static const double eclips = 0.00001;

    static char* number_to_string(uint64_t num, char* str)
    {
        uint64_t next = num / 10;
        if (next)
            str = number_to_string(next, str) + 1;

        *str = (0x30 + (num % 10));
        return str;
    }

    size_t uint8_to_string(uint8_t num, char* str)
    {
        if (!str)
            return 0;

        return number_to_string(num, str) - str + 1;
    }

    size_t int8_to_string(int16_t num, char* str)
    {
        if (!str)
            return 0;

        char * pos = str;

        if (num < 0) { *(pos++) = '-'; num = -num;}

        return number_to_string(num, pos) - str + 1;
    }

    size_t uint16_to_string(uint16_t num, char* str)
    {
        if (!str)
            return 0;

        return number_to_string(num, str) - str + 1;
    }

    size_t int16_to_string(int32_t num, char* str)
    {
        if (!str)
            return 0;

        char * pos = str;

        if (num < 0) { *(pos++) = '-'; num = -num; }

        return number_to_string(num, pos) - str + 1;
    }

    size_t uint32_to_string(uint32_t num, char* str)
    {
        if (!str)
            return 0;

        return number_to_string(num, str) - str + 1;
    }

    size_t int32_to_string(int64_t num, char* str)
    {
        if (!str)
            return 0;

        char * pos = str;

        if (num < 0) { *(pos++) = '-'; num = -num; }

        return number_to_string(num, pos) - str + 1;
    }

    size_t uint64_to_string(uint64_t num, char* str)
    {
        if (!str)
            return 0;

        return number_to_string(num, str) - str + 1;
    }

    size_t int64_to_string(int64_t num, char* str)
    {
        if (!str)
            return 0;

        char * pos = str;

        uint64_t abs_num = num >= 0 ? num : -num;

        if (num < 0) { *(pos++) = '-'; }

        return number_to_string(num, pos) - str + 1;
    }

    size_t double_to_string(double num, char* str, int prec)
    {
        if (!str)
            return 0;

        char* pos =  str;

        // make the default precious to 14
        if (prec <= 0) prec = 14;

        if (num < 0)
        {
            *(++pos) = '-';
            num = -num;
        }

        uint64_t integer = num;

        pos = number_to_string(integer, pos);

        double fraction = num - integer;

        if (prec > 0) *(++pos) = '.';

        int org_prec = prec;
        do
        {
            // this is expensive... it could be only one multiply and then
            // print it as an interger when we know extract the precision.
            fraction *= 10;
            integer = (uint64_t)fraction;
            *(++pos) = integer + 0x30;
            fraction = fraction - integer;
            prec--;
        }
        while (prec > 0);
        prec = org_prec;

        if (fraction > 0.5)
        {
            char * rev = pos;

            do
            {
                *rev += 1;
            } while ((*rev != '.') && (*rev) > '9' && ((*rev='0') && --rev));
        }

        while ('0' == *pos && (prec--) > 1) *(pos--) = 0;

        return pos - str + 1;
    }

}} // tp:utils
