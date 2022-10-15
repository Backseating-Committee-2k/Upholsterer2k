#include <stdbool.h>
#include <assert.h>
#include "upholsterer2k/string_view_utils.h"

static Word char_value(char c) {
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
            return (Word)(c - '0');
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

static StringView strip_prefix(StringView const string_view) {
    return (StringView){
        .data = string_view.data + 2,
        .length = string_view.length - 2,
    };
}

static size_t max_allowed_length(Base const base) {
    switch (base) {
        case BASE_BINARY:
            return 32;
        case BASE_DECIMAL:
            return 10;
        case BASE_HEXADECIMAL:
            return 8;
        default:
            assert(false && "invalid base");
            break;
    }
    return 0;
}

void parse_word_with_base(StringView const string_view, Base const base, bool* out_success, Word* out_result) {
    uint64_t current_factor = 1;
    uint64_t result = 0;
    size_t const max_length = max_allowed_length(base);
    if (string_view.length > max_length) {
        *out_success = false;
        return;
    }
    for (size_t i = 0; i < string_view.length; ++i) {
        size_t const index = string_view.length - i - 1;
        result += char_value(string_view.data[index]) * current_factor;
        current_factor *= (uint64_t)base;
    }
    if (result > 0xFFFFFFFF) {
        *out_success = false;
        return;
    }
    *out_success = true;
    *out_result = (Word)result;
}

void parse_word(StringView const string_view, bool* out_success, Word* out_result) {
    bool const may_have_prefix = string_view.length > 2;
    bool conversion_success;
    Word conversion_result;
    if (may_have_prefix) {
        switch (string_view.data[1]) {
            case 'x':
                parse_word_with_base(
                    strip_prefix(string_view),
                    BASE_HEXADECIMAL,
                    &conversion_success,
                    &conversion_result
                );
                break;
            case 'b':
                parse_word_with_base(
                    strip_prefix(string_view),
                    BASE_BINARY,
                    &conversion_success,
                    &conversion_result
                );
                break;
            default:
                parse_word_with_base(
                    string_view,
                    BASE_DECIMAL,
                    &conversion_success,
                    &conversion_result
                );
                break;
        }
    } else {
        parse_word_with_base(
            string_view,
            BASE_DECIMAL,
            &conversion_success,
            &conversion_result
        );
    }
    if (conversion_success) {
        *out_result = conversion_result;
    }
    *out_success = conversion_success;
}
