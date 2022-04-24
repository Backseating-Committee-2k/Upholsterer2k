#include "parser.h"
#include "error.h"
#include "types.h"
#include "constants.h"
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>

CREATE_VECTOR_DEFINITION(ByteVector, uint8_t, byte_vector)

typedef struct {
    ArgumentType type;
    Token const* first_token;
} Argument;

typedef struct {
    StringView name;
    Address address;
} Label;

CREATE_VECTOR_DECLARATION(LabelVector, Label, label_vector)
CREATE_VECTOR_DEFINITION(LabelVector, Label, label_vector)

CREATE_VECTOR_DECLARATION(ArgumentVector, Argument, argument_vector)
CREATE_VECTOR_DEFINITION(ArgumentVector, Argument, argument_vector)

typedef struct {
    TokenVector tokens;
    OpcodeList opcodes;
    size_t current;
    ByteVector machine_code;
    LabelVector labels;
    SourceFile source_file;
} ParserState;

typedef struct {
    uint8_t bytes[4];
} U32Bytes;

typedef struct {
    uint8_t bytes[8];
} U64Bytes;

static ParserState state;

static Token* current(void) {
    return &state.tokens.data[state.current];
}

static Token* peek(void) {
    return state.current == state.tokens.size - 1 ? NULL : &state.tokens.data[state.current + 1];
}

static Token* next(void) {
    if (state.current == state.tokens.size - 1) {
        return NULL;
    }
    ++state.current;
    return current();
}

static U32Bytes u32_to_big_endian(uint32_t value) {
    return (U32Bytes){
        .bytes = {
            (uint8_t)(value >> 24),
            (uint8_t)(value >> 16),
            (uint8_t)(value >> 8),
            (uint8_t)(value >> 0),
        }
    };
}

static U64Bytes u64_to_big_endian(uint64_t value) {
    return (U64Bytes){
        .bytes = {
            (uint8_t)(value >> 56),
            (uint8_t)(value >> 48),
            (uint8_t)(value >> 40),
            (uint8_t)(value >> 32),
            (uint8_t)(value >> 24),
            (uint8_t)(value >> 16),
            (uint8_t)(value >> 8),
            (uint8_t)(value >> 0),
        }
    };
}

static void emit_u32(uint32_t value) {
    assert(state.machine_code.size % 4 == 0 && "invalid alignment");
    U32Bytes const bytes = u32_to_big_endian(value);
    byte_vector_push(&state.machine_code, bytes.bytes[0]);
    byte_vector_push(&state.machine_code, bytes.bytes[1]);
    byte_vector_push(&state.machine_code, bytes.bytes[2]);
    byte_vector_push(&state.machine_code, bytes.bytes[3]);
}

static void emit_u64(uint64_t value) {
    assert(state.machine_code.size % 4 == 0 && "invalid alignment");
    if (state.machine_code.size % 8 != 0) {
        // fix alignment by inserting padding
        emit_u32(0xDEADC0DE);
    }
    assert(state.machine_code.size % 8 == 0 && "invalid alignment");
    U64Bytes const bytes = u64_to_big_endian(value);
    byte_vector_push(&state.machine_code, bytes.bytes[0]);
    byte_vector_push(&state.machine_code, bytes.bytes[1]);
    byte_vector_push(&state.machine_code, bytes.bytes[2]);
    byte_vector_push(&state.machine_code, bytes.bytes[3]);
    byte_vector_push(&state.machine_code, bytes.bytes[4]);
    byte_vector_push(&state.machine_code, bytes.bytes[5]);
    byte_vector_push(&state.machine_code, bytes.bytes[6]);
    byte_vector_push(&state.machine_code, bytes.bytes[7]);
}

static bool register_label(StringView label_name) {
    for (size_t i = 0; i < state.labels.size; ++i) {
        if (string_view_compare(label_name, state.labels.data[i].name) == 0) {
            return false;
        }
    }
    label_vector_push(&state.labels, (Label){
        .name = label_name,
        .address = (Address)(state.machine_code.size + ENTRY_POINT),
    });
    return true;
}

static void error_on_token(char const * const message, Token const * const token) {
    error(
        state.source_file,
        message,
        token->line,
        token->column,
        token->string_view.length
    );
}

static void error_on_current_token(char const * const message) {
    error_on_token(message, current());
}

static void init_state(SourceFile const source_file, TokenVector const tokens, OpcodeList const opcodes) {
    state = (ParserState){
        .tokens = tokens,
        .opcodes = opcodes,
        .current = 0,
        .machine_code = byte_vector_create(),
        .labels = label_vector_create(),
        .source_file = source_file,
    };
}

static ByteVector cleanup_state(void) {
    label_vector_free(&state.labels);
    return state.machine_code;
}

static bool do_arguments_match(ArgumentType const lhs, ArgumentType const rhs) {
    return lhs == rhs
        || (lhs == ARGUMENT_TYPE_ADDRESS && rhs == ARGUMENT_TYPE_LABEL)
        || (lhs == ARGUMENT_TYPE_LABEL && rhs == ARGUMENT_TYPE_ADDRESS);
}

static bool do_argument_lists_match(
    ArgumentVector const arguments,
    OpcodeSpecification const * const opcode
) {
    assert(arguments.size == opcode->argument_count);
    for (size_t i = 0; i < arguments.size; ++i) {
        if (!do_arguments_match(arguments.data[i].type, opcode->required_arguments[arguments.size - i - 1])) {
            return false;
        }
    }
    return true;
}

static OpcodeSpecification const* find_opcode(Token const * const mnemonic, ArgumentVector const arguments) {
    for (size_t i = 0; i < state.opcodes.num_specifications; ++i) {
        OpcodeSpecification const * const opcode = &state.opcodes.specifications[i];
        if (
            opcode->argument_count == arguments.size
            && string_view_compare_case_insensitive(mnemonic->string_view, opcode->mnemonic) == 0
            && do_argument_lists_match(arguments, opcode)
        ) {
            return opcode;
        }
    }
    return NULL;
}

static void emit_instruction(Token const * const mnemonic, ArgumentVector const arguments) {
    fprintf(
        stderr,
        "should emit instruction for mnemonic %.*s with %zu arguments.\n",
        (int)mnemonic->string_view.length,
        mnemonic->string_view.data,
        arguments.size
    );
    for (size_t i = 0; i < arguments.size; ++i) {
        fprintf(
            stderr,
            "\t%.*s\n",
            (int)arguments.data[i].first_token->string_view.length,
            arguments.data[i].first_token->string_view.data
        );
    }
    OpcodeSpecification const * const opcode_specification = find_opcode(mnemonic, arguments);
    if (opcode_specification == NULL) {
        error_on_token("unknown instruction or invalid arguments", mnemonic);
    } else {
        fprintf(
            stderr,
            "\tfound matching opcode: %.*s (%#.04x)\n",
            (int)opcode_specification->name.length,
            opcode_specification->name.data,
            opcode_specification->opcode
        );
    }

    Instruction instruction = ((Instruction)opcode_specification->opcode) << 48;

    bool success;
    Word word_result;
    Register register_result;
    size_t shift_offset = 40;
    for (size_t i = 0; i < arguments.size; ++i) {
        size_t index = arguments.size - i - 1;
        switch (arguments.data[index].type) {
            case ARGUMENT_TYPE_ADDRESS:
                assert(arguments.data[index].first_token->type == TOKEN_TYPE_ASTERISK);
                word_from_token(arguments.data[index].first_token + 1, &success, &word_result);
                fprintf(stderr, "%.*s\n", (int)(arguments.data[index].first_token + 1)->string_view.length,
                (arguments.data[index].first_token + 1)->string_view.data);
                assert(success);
                instruction |= word_result;
                break;
            case ARGUMENT_TYPE_IMMEDIATE:
                word_from_token(arguments.data[index].first_token, &success, &word_result);
                assert(success); // should've been checked before
                instruction |= word_result;
                break;
            case ARGUMENT_TYPE_LABEL:
                assert(false && "not implemented");
                break;
            case ARGUMENT_TYPE_NONE:
                assert(false && "unreachable");
                break;
            case ARGUMENT_TYPE_POINTER:
                assert(arguments.data[index].first_token->type == TOKEN_TYPE_ASTERISK);
                register_from_token(arguments.data[index].first_token + 1, &success, &register_result);
                assert(success);
                instruction |= ((Instruction)register_result) << shift_offset;
                shift_offset -= 8;
                break;
            case ARGUMENT_TYPE_REGISTER:
                register_from_token(arguments.data[index].first_token, &success, &register_result);
                assert(success);
                instruction |= ((Instruction)register_result) << shift_offset;
                shift_offset -= 8;
                break;
        }
    }

    emit_u64(instruction);
}

static void parse_label(void) {
    assert(peek()->type == TOKEN_TYPE_COLON);
    if (!register_label(identifier_from_token(current()))) {
        error_on_current_token("label redefinition");
    }
    next(); // proceed to colon token
    if (peek()->type != TOKEN_TYPE_NEWLINE) {
        error_on_current_token("newline expected after label definition");
    }
}

static void parse_instruction(void) {
    Token const * const mnemonic = current();
    Token const* current_argument_start = NULL;
    ArgumentVector arguments = argument_vector_create();
    bool valid_argument_start_position = true;
    next();
    while (true) {
        if (current()->type == TOKEN_TYPE_COMMA) {
            if (valid_argument_start_position) {
                error_on_current_token("unexpected comma");
            } else {
                next();
                valid_argument_start_position = true;
            }
        }
        if (current()->type == TOKEN_TYPE_ASTERISK) {
            if (!valid_argument_start_position) {
                error_on_current_token("comma expected");
            }
            // pointer or address
            current_argument_start = current();
        } else if (current()->type == TOKEN_TYPE_EOF) {
            error(state.source_file, "unexpected end of file", current()->line, current()->column, 1);
        } else if (current()->type == TOKEN_TYPE_NEWLINE) {
            if (current_argument_start != NULL) {
                error_on_current_token("register or address expected");
            }
            break;
        } else {
            if (current_argument_start == NULL) {
                if (!valid_argument_start_position) {
                    error_on_current_token("comma expected");
                }
                // no pointer and no address
                switch (current()->type) {
                    case TOKEN_TYPE_WORD_LITERAL:
                        argument_vector_push(&arguments, (Argument){
                            .type = ARGUMENT_TYPE_IMMEDIATE,
                            .first_token = current(),
                        });
                        valid_argument_start_position = false;
                        break;
                    case TOKEN_TYPE_REGISTER:
                        argument_vector_push(&arguments, (Argument){
                            .type = ARGUMENT_TYPE_REGISTER,
                            .first_token = current(),
                        });
                        valid_argument_start_position = false;
                        break;
                    case TOKEN_TYPE_IDENTIFIER:
                        argument_vector_push(&arguments, (Argument){
                            .type = ARGUMENT_TYPE_LABEL,
                            .first_token = current(),
                        });
                        valid_argument_start_position = false;
                        break;
                    default:
                        error_on_current_token("invalid argument");
                }
            } else {
                // second token of pointer or address
                switch (current()->type) {
                    case TOKEN_TYPE_WORD_LITERAL:
                        argument_vector_push(&arguments, (Argument){
                            .type = ARGUMENT_TYPE_ADDRESS,
                            .first_token = current_argument_start,
                        });
                        valid_argument_start_position = false;
                        break;
                    case TOKEN_TYPE_REGISTER:
                        argument_vector_push(&arguments, (Argument){
                            .type = ARGUMENT_TYPE_POINTER,
                            .first_token = current_argument_start,
                        });
                        valid_argument_start_position = false;
                        break;
                    default:
                        error_on_current_token("invalid argument");
                }
                current_argument_start = NULL;
            }
        }
        next();
    }
    emit_instruction(mnemonic, arguments);
    argument_vector_free(&arguments);
}

static void parse_identifier(void) {
    assert(current()->type == TOKEN_TYPE_IDENTIFIER);
    if (peek()->type == TOKEN_TYPE_COLON) {
        parse_label();
    } else {
        parse_instruction();
    }
}

static void parse_word_literal(void) {
    if (current()->type != TOKEN_TYPE_WORD_LITERAL) {
        error_on_current_token("expected word literal");
    }
    bool success;
    Word result;
    word_from_token(current(), &success, &result);
    assert(success); // lexer should only tokenize word literals if they are valid
    emit_u32(result);
    fprintf(stderr, "should emit word literal: %"PRIu32"\n", result);
    next();
}

static void parse_words_literal(void) {
    assert(current()->type == TOKEN_TYPE_IDENTIFIER);
    assert(string_view_compare_case_insensitive(current()->string_view, string_view_from_string("words")) == 0);
    next();
    if (current()->type != TOKEN_TYPE_LEFT_BRACKET) {
        error_on_current_token("\"[\" expected");
    }
    next();
    parse_word_literal();
    while (current()->type == TOKEN_TYPE_COMMA) {
        next();
        if (current()->type == TOKEN_TYPE_RIGHT_BRACKET) {
            break;
        }
        parse_word_literal();
    }
    if (current()->type != TOKEN_TYPE_RIGHT_BRACKET) {
        error_on_current_token("\"]\" expected");
    }
    next();
    if (current()->type != TOKEN_TYPE_NEWLINE) {
        error_on_current_token("newline expected after words literal");
    }
}

static void emit_quoted_escaped_string(StringView string) {
    char const* current = string.data + 1;
    char const * const end = string.data + string.length - 1;
    while (current != end) {
        switch (*current) {
            case '\\':
                ++current;
                assert(current != end); // the lexer already checks for valid escaping
                switch (*current) {
                    case '"':
                        emit_u32((Word)'"');
                        break;
                    case '\\':
                        emit_u32((Word)'\\');
                        break;
                    case 't':
                        emit_u32((Word)'\t');
                        break;
                    case 'n':
                        emit_u32((Word)'\n');
                        break;
                    case 'v':
                        emit_u32((Word)'\v');
                        break;
                    case 'f':
                        emit_u32((Word)'\f');
                        break;
                    case 'r':
                        emit_u32((Word)'\r');
                        break;
                    default:
                        assert(false && "invalid escape sequence"); // should be caught by the lexer
                        break;
                }
                break;
            default:
                emit_u32((Word)*current);
                break;
        }
        ++current;
    }
}

static void parse_string_literal(void) {
    assert(current()->type == TOKEN_TYPE_IDENTIFIER);
    assert(string_view_compare_case_insensitive(current()->string_view, string_view_from_string("string")) == 0);
    next();
    if (current()->type != TOKEN_TYPE_STRING_LITERAL) {
        error_on_current_token("string literal expected");
    }
    fprintf(stderr, "should emit string literal: %.*s\n", (int)current()->string_view.length, current()->string_view.data);
    emit_quoted_escaped_string(current()->string_view);
    next();
    if (current()->type != TOKEN_TYPE_NEWLINE) {
        error_on_current_token("newline expected after string literal");
    }
}

static void parse_literal(void) {
    assert(current()->type == TOKEN_TYPE_DOT);
    next();
    if (current()->type != TOKEN_TYPE_IDENTIFIER) {
        error_on_current_token("expected literal");
    }

    if (string_view_compare_case_insensitive(current()->string_view, string_view_from_string("words")) == 0) {
        parse_words_literal();
    } else if (string_view_compare_case_insensitive(current()->string_view, string_view_from_string("string")) == 0) {
        parse_string_literal();
    } else {
        error_on_current_token("expected either \"words\" or \"string\"");
    }
}

ByteVector parse(SourceFile const source_file, TokenVector const tokens, OpcodeList const opcodes) {
    assert(tokens.size > 0);
    assert(tokens.data[tokens.size - 1].type == TOKEN_TYPE_EOF);
    if (tokens.size == 1) {
        error(source_file, "empty input", tokens.data[0].line, tokens.data[0].column, 1);
    }
    init_state(source_file, tokens, opcodes);
    while (current()->type != TOKEN_TYPE_EOF) {
        switch (current()->type) {
            case TOKEN_TYPE_IDENTIFIER:
                parse_identifier();
                break;
            case TOKEN_TYPE_DOT:
                parse_literal();
                break;
            case TOKEN_TYPE_NEWLINE:
                break;
            default:
                error_on_current_token("unexpected token");
                break;
        }
        next();
    }
    return cleanup_state();
}
