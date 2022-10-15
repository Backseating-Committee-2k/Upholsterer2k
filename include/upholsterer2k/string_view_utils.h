#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "upholsterer2k/types.h"
#include "upholsterer2k/string_view.h"

typedef enum {
    BASE_DECIMAL = 10,
    BASE_HEXADECIMAL = 16,
    BASE_BINARY = 2,
} Base;

void parse_word(StringView string_view, bool* out_success, Word* out_result);
void parse_word_with_base(StringView const string_view, Base const base, bool* out_success, Word* out_result);

#ifdef __cplusplus
}
#endif
