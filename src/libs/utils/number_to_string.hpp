#ifndef __tp_utils_number_to_string_hpp__
#define __tp_utils_number_to_string_hpp__

#include <cstring>

extern "C" {
#include <stdint.h>
}

namespace tp {
namespace utils {

    size_t uint8_to_string(uint8_t num, char* str);

    size_t int8_to_string(int16_t num, char* str);

    size_t uint16_to_string(uint16_t num, char* str);

    size_t int16_to_string(int32_t num, char* str);

    size_t uint32_to_string(uint32_t num, char* str);

    size_t int32_to_string(int64_t num, char* str);

    size_t uint64_to_string(uint64_t num, char* str);

    size_t int64_to_string(int64_t num, char* str);

    size_t double_to_string(double num, char* str, int prec = -1);

}} // tp::utils

#endif//__tp_utils_number_to_string_hpp__
