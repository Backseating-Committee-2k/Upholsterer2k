#include <assert.h>
#include <ctype.h>
#include "upholsterer2k/token.h"
#include "upholsterer2k/string_view_utils.h"

char const* UP2K_token_type_to_string(UP2K_TokenType token_type) {
    switch (token_type) {
        #define X(type) case type : return #type ;
        TOKEN_TYPE_LIST
        #undef X
        default:
            assert(false && "unreachable");
            return NULL;
    }
}

#undef TOKEN_TYPE_CASE

UP2K_StringView UP2K_identifier_from_token(UP2K_Token const * const token) {
    assert(token->type == TOKEN_TYPE_IDENTIFIER);
    return token->string_view;
}

UP2K_StringView UP2K_string_literal_from_token(UP2K_Token const * const token) {
    assert(token->type == TOKEN_TYPE_STRING_LITERAL);
    return (UP2K_StringView){
        .data = token->string_view.data + 1,
        .length = token->string_view.length - 2,
    };
}

void UP2K_word_from_token(UP2K_Token const * const token, bool* const out_success, UP2K_Word* const out_result) {
    assert(token->type == TOKEN_TYPE_WORD_LITERAL);
    UP2K_parse_word(token->string_view, out_success, out_result);
}

void UP2K_register_from_token(UP2K_Token const * const token, bool* const out_success, UP2K_Register* const out_register) {
    assert(token->type == TOKEN_TYPE_REGISTER);
    if (token->string_view.length > 4) {
        *out_success = false;
        return;
    }
    int result = 0;
    int UP2K_Base = 1;
    for (size_t i = token->string_view.length - 1; i > 0; --i) {
        assert(isdigit(token->string_view.data[i]));
        int const digit = (token->string_view.data[i] - '0');
        result += digit * UP2K_Base;
        UP2K_Base *= 10;
    }
    assert(result >= 0);
    if (result > 255) {
        *out_success = false;
        return;
    }
    *out_register = (UP2K_Register)result;
    *out_success = true;
}
