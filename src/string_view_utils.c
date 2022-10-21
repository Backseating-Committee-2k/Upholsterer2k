#include <stdbool.h>
#include <assert.h>
#include "upholsterer2k/string_view_utils.h"

static UP2K_Word char_value(char c) {
    switch (c) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return (UP2K_Word)(c - '0');
        case 'a':
        case 'A':
            return 10;
        case 'b':
        case 'B':
            return 11;
        case 'c':
        case 'C':
            return 12;
        case 'd':
        case 'D':
            return 13;
        case 'e':
        case 'E':
            return 14;
        case 'f':
        case 'F':
            return 15;
        default:
            assert(false && "unexpected character in string");
            break;
    }
    return 0;
}

static UP2K_StringView strip_prefix(UP2K_StringView const string_view) {
    return (UP2K_StringView){
        .data = string_view.data + 2,
        .length = string_view.length - 2,
    };
}

static size_t max_allowed_length(UP2K_Base const UP2K_Base) {
    switch (UP2K_Base) {
        case UP2K_BASE_BINARY:
            return 32;
        case UP2K_BASE_DECIMAL:
            return 10;
        case UP2K_BASE_HEXADECIMAL:
            return 8;
        default:
            assert(false && "invalid UP2K_Base");
            break;
    }
    return 0;
}

void UP2K_parse_word_with_base(UP2K_StringView const string_view, UP2K_Base const UP2K_Base, bool* out_success, UP2K_Word* out_result) {
    uint64_t current_factor = 1;
    uint64_t result = 0;
    size_t const max_length = max_allowed_length(UP2K_Base);
    if (string_view.length > max_length) {
        *out_success = false;
        return;
    }
    for (size_t i = 0; i < string_view.length; ++i) {
        size_t const index = string_view.length - i - 1;
        result += char_value(string_view.data[index]) * current_factor;
        current_factor *= (uint64_t)UP2K_Base;
    }
    if (result > 0xFFFFFFFF) {
        *out_success = false;
        return;
    }
    *out_success = true;
    *out_result = (UP2K_Word)result;
}

void UP2K_parse_word(UP2K_StringView const string_view, bool* out_success, UP2K_Word* out_result) {
    bool const may_have_prefix = string_view.length > 2;
    bool conversion_success;
    UP2K_Word conversion_result;
    if (may_have_prefix) {
        switch (string_view.data[1]) {
            case 'x':
                UP2K_parse_word_with_base(
                    strip_prefix(string_view),
                    UP2K_BASE_HEXADECIMAL,
                    &conversion_success,
                    &conversion_result
                );
                break;
            case 'b':
                UP2K_parse_word_with_base(
                    strip_prefix(string_view),
                    UP2K_BASE_BINARY,
                    &conversion_success,
                    &conversion_result
                );
                break;
            default:
                UP2K_parse_word_with_base(
                    string_view,
                    UP2K_BASE_DECIMAL,
                    &conversion_success,
                    &conversion_result
                );
                break;
        }
    } else {
        UP2K_parse_word_with_base(
            string_view,
            UP2K_BASE_DECIMAL,
            &conversion_success,
            &conversion_result
        );
    }
    if (conversion_success) {
        *out_result = conversion_result;
    }
    *out_success = conversion_success;
}
