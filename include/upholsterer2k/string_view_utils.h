#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "upholsterer2k/types.h"
#include "upholsterer2k/string_view.h"

typedef enum {
    UP2K_BASE_DECIMAL = 10,
    UP2K_BASE_HEXADECIMAL = 16,
    UP2K_BASE_BINARY = 2,
} UP2K_Base;

void UP2K_parse_word(UP2K_StringView string_view, bool* out_success, UP2K_Word* out_result);
void UP2K_parse_word_with_base(UP2K_StringView const string_view, UP2K_Base const UP2K_Base, bool* out_success, UP2K_Word* out_result);

#ifdef __cplusplus
}
#endif
