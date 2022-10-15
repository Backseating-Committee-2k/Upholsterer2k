#include "upholsterer2k/hashmap.h"

#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define FNV_OFFSET_BASIS 0xcbf29ce484222325
#define FNV_PRIME 0x100000001b3

size_t const powers_of_two[64] = {
    ((size_t)1) << 0,
    ((size_t)1) << 1,
    ((size_t)1) << 2,
    ((size_t)1) << 3,
    ((size_t)1) << 4,
    ((size_t)1) << 5,
    ((size_t)1) << 6,
    ((size_t)1) << 7,
    ((size_t)1) << 8,
    ((size_t)1) << 9,
    ((size_t)1) << 10,
    ((size_t)1) << 11,
    ((size_t)1) << 12,
    ((size_t)1) << 13,
    ((size_t)1) << 14,
    ((size_t)1) << 15,
    ((size_t)1) << 16,
    ((size_t)1) << 17,
    ((size_t)1) << 18,
    ((size_t)1) << 19,
    ((size_t)1) << 20,
    ((size_t)1) << 21,
    ((size_t)1) << 22,
    ((size_t)1) << 23,
    ((size_t)1) << 24,
    ((size_t)1) << 25,
    ((size_t)1) << 26,
    ((size_t)1) << 27,
    ((size_t)1) << 28,
    ((size_t)1) << 29,
    ((size_t)1) << 30,
    ((size_t)1) << 31,
    ((size_t)1) << 32,
    ((size_t)1) << 33,
    ((size_t)1) << 34,
    ((size_t)1) << 35,
    ((size_t)1) << 36,
    ((size_t)1) << 37,
    ((size_t)1) << 38,
    ((size_t)1) << 39,
    ((size_t)1) << 40,
    ((size_t)1) << 41,
    ((size_t)1) << 42,
    ((size_t)1) << 43,
    ((size_t)1) << 44,
    ((size_t)1) << 45,
    ((size_t)1) << 46,
    ((size_t)1) << 47,
    ((size_t)1) << 48,
    ((size_t)1) << 49,
    ((size_t)1) << 50,
    ((size_t)1) << 51,
    ((size_t)1) << 52,
    ((size_t)1) << 53,
    ((size_t)1) << 54,
    ((size_t)1) << 55,
    ((size_t)1) << 56,
    ((size_t)1) << 57,
    ((size_t)1) << 58,
    ((size_t)1) << 59,
    ((size_t)1) << 60,
    ((size_t)1) << 61,
    ((size_t)1) << 62,
    ((size_t)1) << 63,
};

// source: https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
uint64_t hash_string_view(StringView string_view) {
    uint64_t hash = FNV_OFFSET_BASIS;
    for (size_t i = 0; i < string_view.length; ++i) {
        hash *= FNV_PRIME;
        hash ^= (uint64_t)tolower(string_view.data[i]); // case-insensitive due to call to tolower
    }
    return hash;
}
