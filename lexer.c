#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "upholsterer2k/lexer.h"
#include "upholsterer2k/error.h"
#include "upholsterer2k/constants.h"

typedef enum {
    DECIMAL,
    HEXADECIMAL,
    BINARY,
    UNKNOWN,
} WordLiteralType;

static size_t minimum_required_length(WordLiteralType type) {
    switch (type) {
        case DECIMAL:
            return 1;
        case HEXADECIMAL:
        case BINARY:
            return 3;
        default:
            return 0;
    }
}

static bool is_valid_digit(char const c, WordLiteralType base) {
    switch (base) {
        case DECIMAL:
            return isdigit(c);
        case HEXADECIMAL:
            return isxdigit(c);
        case BINARY:
            return c == '0' || c == '1';
        case UNKNOWN:
            return false;
        default:
            assert(false && "invalid base");
            return false;
    }
}

static bool is_valid_identifier_start_char(char const c) {
    return isalpha(c) || c == '_' || c == '$' || c == '%';
}

static bool is_valid_identifier_inner_char(char const c) {
    return is_valid_identifier_start_char(c) || isdigit(c) || c == '-';
}

static bool is_valid_string_literal_char(char const c) {
    return isprint(c) && (!isspace(c) || c == ' ');
}

static bool is_escape_sequence_char(char const c) {
    return c == '"' || c == '\\' || c == 't' || c == 'n' || c == 'v' || c == 'f' || c == 'r';
}

TokenVector tokenize(SourceFile const source_file, ConstantsMap const* constants) {
    TokenVector tokens = token_vector_create();
    if (source_file.source.length == 0) {
        return tokens;
    }
    char const* current = source_file.source.data;
    char const* const end = source_file.source.data + source_file.source.length;
    char const* current_line_start = current;
    size_t line = 1;
    while (current != end) {
        switch (*current) {
            case '\n':
                if (tokens.size == 0 || tokens.data[tokens.size - 1].type != TOKEN_TYPE_NEWLINE) {
                    token_vector_push(&tokens, (Token){
                        .type = TOKEN_TYPE_NEWLINE,
                        .string_view = {
                            .data = current,
                            .length = 1,
                        },
                        .column = (size_t)(current - current_line_start + 1),
                        .line = line,
                    });
                }
                ++line;
                current_line_start = current + 1;
                break;
            case '*':
                token_vector_push(&tokens, (Token){
                    .type = TOKEN_TYPE_ASTERISK,
                    .string_view = {
                        .data = current,
                        .length = 1,
                    },
                    .column = (size_t)(current - current_line_start + 1),
                    .line = line,
                });
                break;
            case ':':
                token_vector_push(&tokens, (Token){
                    .type = TOKEN_TYPE_COLON,
                    .string_view = {
                        .data = current,
                        .length = 1,
                    },
                    .column = (size_t)(current - current_line_start + 1),
                    .line = line,
                });
                break;
            case '.':
                token_vector_push(&tokens, (Token){
                    .type = TOKEN_TYPE_DOT,
                    .string_view = {
                        .data = current,
                        .length = 1,
                    },
                    .column = (size_t)(current - current_line_start + 1),
                    .line = line,
                });
                break;
            case ',':
                token_vector_push(&tokens, (Token){
                    .type = TOKEN_TYPE_COMMA,
                    .string_view = {
                        .data = current,
                        .length = 1,
                    },
                    .column = (size_t)(current - current_line_start + 1),
                    .line = line,
                });
                break;
            case '[':
                token_vector_push(&tokens, (Token){
                    .type = TOKEN_TYPE_LEFT_BRACKET,
                    .string_view = {
                        .data = current,
                        .length = 1,
                    },
                    .column = (size_t)(current - current_line_start + 1),
                    .line = line,
                });
                break;
            case ']':
                token_vector_push(&tokens, (Token){
                    .type = TOKEN_TYPE_RIGHT_BRACKET,
                    .string_view = {
                        .data = current,
                        .length = 1,
                    },
                    .column = (size_t)(current - current_line_start + 1),
                    .line = line,
                });
                break;
            case '/':
                ++current;
                if (current == end || *current != '/') {
                    error(
                        source_file,
                        "'/' expected",
                        line,
                        (size_t)(current - current_line_start + 1),
                        1
                    );
                }
                while (current != end && *current != '\n') {
                    ++current;
                }
                if (current == end) {
                    error(
                        source_file,
                        "comments must end with newline",
                        line,
                        (size_t)(current - current_line_start + 1),
                        1
                    );
                }
                --current;
                break;
            case '"': {
                // string literals
                char const * const literal_start = current;
                ++current;
                while (current != end && *current != '"' && is_valid_string_literal_char(*current)) {
                    if (*current == '\\') {
                        // escape sequence
                        if (current + 1 == end || !is_escape_sequence_char(*(current + 1))) {
                            error(
                                source_file,
                                "invalid escape sequence",
                                line,
                                (size_t)(current - current_line_start + 1),
                                1
                            );
                        }
                        ++current;
                    }
                    ++current;
                }
                if (!is_valid_string_literal_char(*current)) {
                    error(
                        source_file,
                        "character not allowed in string literal",
                        line,
                        (size_t)(current - current_line_start + 1),
                        1
                    );
                }
                if (current == end || *current != '"') {
                    error(
                        source_file,
                        "unterminated string literal",
                        line,
                        (size_t)(current - current_line_start + 1),
                        1
                    );
                }
                ++current;
                token_vector_push(&tokens, (Token){
                    .type = TOKEN_TYPE_STRING_LITERAL,
                    .string_view = string_view_from_pointers(literal_start, current),
                    .column = (size_t)(literal_start - current_line_start + 1),
                    .line = line,
                });
                --current;
                break;
            }
            default:
                if (isspace(*current)) {
                    // do nothing
                } else if ((*current == 'r' || *current == 'R') && current + 1 != end && isdigit(*(current + 1))) {
                    // register
                    char const * const register_start = current;
                    ++current;
                    while (current != end && isdigit(*current)) {
                        ++current;
                    }
                    token_vector_push(&tokens, (Token){
                        .type = TOKEN_TYPE_REGISTER,
                        .string_view = string_view_from_pointers(register_start, current),
                        .line = line,
                        .column = (size_t)(register_start - current_line_start + 1),
                    });
                    --current;
                } else if (isdigit(*current)) {
                    // word literal
                    if (current + 1 == end) {
                        // reached EOF
                        token_vector_push(&tokens, (Token){
                            .type = TOKEN_TYPE_WORD_LITERAL,
                            .string_view = {
                                .data = current,
                                .length = 1,
                            },
                            .column = (size_t)(current - current_line_start + 1),
                            .line = line,
                        });
                    } else {
                        char const * const literal_start = current;
                        ++current;
                        WordLiteralType word_literal_type;
                        switch (*current) {
                            case 'x':
                                ++current;
                                word_literal_type = HEXADECIMAL;
                                break;
                            case 'b':
                                ++current;
                                word_literal_type = BINARY;
                                break;
                            default:
                                word_literal_type = isdigit(*current) ? DECIMAL : UNKNOWN;
                                break;
                        }
                        while (current != end && is_valid_digit(*current, word_literal_type)) {
                            ++current;
                        }
                        size_t const literal_length = (size_t)(current - literal_start);
                        if (literal_length < minimum_required_length(word_literal_type)) {
                            error(
                                source_file,
                                "end of word literal unexpected",
                                line,
                                (size_t)(current - current_line_start + 1),
                                1
                            );
                        }
                        token_vector_push(&tokens, (Token){
                            .type = TOKEN_TYPE_WORD_LITERAL,
                            .string_view = {
                                .data = literal_start,
                                .length = literal_length,
                            },
                            .column = (size_t)(literal_start - current_line_start + 1),
                            .line = line,
                        });
                        --current;
                    }
                } else if (current < end - 1 && *current == '$' && *(current + 1) == '"') {
                    // identifier with string literal inside
                    char const * const identifier_start = current;
                    current += 2; // consume '$' and '"'
                    while (current != end && *current != '"') {
                        if (*current < (char)32 || *current > (char)126) {
                            error(
                                source_file,
                                "invalid character inside string literal",
                                line,
                                (size_t)(current - current_line_start + 1),
                                1);
                        }
                        ++current;
                    }
                    if (current == end) {
                        error(
                            source_file,
                            "unterminated string literal",
                            line,
                            (size_t)(current - current_line_start + 1),
                            1);
                    }
                    assert(*current == '"');
                    ++current;
                    size_t const identifier_length = (size_t)(current - identifier_start);
                    token_vector_push(&tokens, (Token){
                            .type = TOKEN_TYPE_IDENTIFIER,
                            .string_view = {
                                .data = identifier_start,
                                .length = identifier_length,
                            },
                            .column = (size_t)(identifier_start - current_line_start + 1),
                            .line = line,
                        });
                    --current;
                } else if (is_valid_identifier_start_char(*current)) {
                    // identifier or register constant or word constant
                    char const * const identifier_start = current;
                    ++current;
                    while (current != end && is_valid_identifier_inner_char(*current)) {
                        ++current;
                    }
                    StringView const identifier = string_view_from_pointers(identifier_start, current);
                    bool constant_found;
                    get_constant_value(
                        identifier,
                        CONSTANT_TYPE_REGISTER,
                        &constant_found,
                        NULL,
                        constants);
                    TokenType token_type = TOKEN_TYPE_IDENTIFIER;
                    if (constant_found) {
                        token_type = TOKEN_TYPE_REGISTER_CONSTANT;
                    }
                    if (!constant_found) {
                        get_constant_value(
                            identifier,
                            CONSTANT_TYPE_UNSIGNED_INTEGER,
                            &constant_found,
                            NULL,
                            constants);
                        if (constant_found) {
                            token_type = TOKEN_TYPE_WORD_CONSTANT;
                        }
                    }
                    token_vector_push(&tokens, (Token){
                        .type = token_type,
                        .string_view = identifier,
                        .line = line,
                        .column = (size_t)(identifier_start - current_line_start + 1),
                    });
                    --current;
                } else {
                    error(
                        source_file,
                        "unexpected input",
                        line,
                        (size_t)(current - current_line_start + 1),
                        1
                    );
                }
                break;
        }
        ++current;
    }
    token_vector_push(&tokens, (Token){
        .type = TOKEN_TYPE_EOF,
        .string_view = {
            .data = NULL,
            .length = 0,
        },
        .column = (size_t)(current - current_line_start + 1),
        .line = line,
    });
    return tokens;
}
