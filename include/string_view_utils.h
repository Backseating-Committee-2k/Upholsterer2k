#pragma once

#include <stdbool.h>
#include "types.h"
#include "string_view.h"

typedef enum {
    BASE_DECIMAL = 10,
    BASE_HEXADECIMAL = 16,
    BASE_BINARY = 2,
} Base;

void parse_word(StringView string_view, bool* out_success, Word* out_result);
void parse_word_with_base(StringView const string_view, Base const base, bool* out_success, Word* out_result);
