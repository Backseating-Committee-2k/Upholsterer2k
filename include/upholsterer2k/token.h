#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdbool.h>
#include "upholsterer2k/string_view.h"
#include "upholsterer2k/types.h"

#define TOKEN_TYPE_LIST             \
    X(TOKEN_TYPE_ASTERISK)          \
    X(TOKEN_TYPE_COLON)             \
    X(TOKEN_TYPE_IDENTIFIER)        \
    X(TOKEN_TYPE_STRING_LITERAL)    \
    X(TOKEN_TYPE_WORD_LITERAL)      \
    X(TOKEN_TYPE_WORD_CONSTANT)     \
    X(TOKEN_TYPE_LEFT_BRACKET)      \
    X(TOKEN_TYPE_RIGHT_BRACKET)     \
    X(TOKEN_TYPE_DOT)               \
    X(TOKEN_TYPE_REGISTER)          \
    X(TOKEN_TYPE_REGISTER_CONSTANT) \
    X(TOKEN_TYPE_COMMA)             \
    X(TOKEN_TYPE_AT)                \
    X(TOKEN_TYPE_NEWLINE)           \
    X(TOKEN_TYPE_EOF)

typedef enum {
#define X(type) type,
    TOKEN_TYPE_LIST
#undef X
} UP2K_TokenType;

char const* UP2K_token_type_to_string(UP2K_TokenType token_type);

typedef struct {
    UP2K_TokenType type;
    UP2K_StringView string_view;
    size_t line, column;
} UP2K_Token;

UP2K_StringView UP2K_identifier_from_token(UP2K_Token const* token);
UP2K_StringView UP2K_string_literal_from_token(UP2K_Token const* token);
void UP2K_word_from_token(UP2K_Token const* token, bool* out_success, UP2K_Word* out_result);
void UP2K_register_from_token(UP2K_Token const* token, bool* out_success, UP2K_Register* out_register);

#ifdef __cplusplus
}
#endif
