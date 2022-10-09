#include <assert.h>
#include <ctype.h>
#include "token.h"
#include "string_view_utils.h"

char const* token_type_to_string(TokenType token_type) {
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

StringView identifier_from_token(Token const * const token) {
    assert(token->type == TOKEN_TYPE_IDENTIFIER);
    return token->string_view;
}

StringView string_literal_from_token(Token const * const token) {
    assert(token->type == TOKEN_TYPE_STRING_LITERAL);
    return (StringView){
        .data = token->string_view.data + 1,
        .length = token->string_view.length - 2,
    };
}

void word_from_token(Token const * const token, bool* const out_success, Word* const out_result) {
    assert(token->type == TOKEN_TYPE_WORD_LITERAL);
    parse_word(token->string_view, out_success, out_result);
}

void register_from_token(Token const * const token, bool* const out_success, Register* const out_register) {
    assert(token->type == TOKEN_TYPE_REGISTER);
    if (token->string_view.length > 4) {
        *out_success = false;
        return;
    }
    int result = 0;
    int base = 1;
    for (size_t i = token->string_view.length - 1; i > 0; --i) {
        assert(isdigit(token->string_view.data[i]));
        int const digit = (token->string_view.data[i] - '0');
        result += digit * base;
        base *= 10;
    }
    assert(result >= 0);
    if (result > 255) {
        *out_success = false;
        return;
    }
    *out_register = (Register)result;
    *out_success = true;
}
