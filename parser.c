#include "upholsterer2k/parser.h"
#include "upholsterer2k/error.h"
#include "upholsterer2k/types.h"
#include "upholsterer2k/constants.h"
#include "upholsterer2k/hashmap.h"
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>
#include <string.h>

CREATE_VECTOR_DEFINITION(UP2K_ByteVector, uint8_t, UP2K_byte_vector)

CREATE_VECTOR_DEFINITION(UP2K_InstructionMapVector, UP2K_InstructionMap, UP2K_instruction_map_vector)

typedef struct {
    UP2K_ArgumentType type;
    UP2K_Token const* first_token;
} Argument;

typedef struct {
    UP2K_StringView name;
    UP2K_Address offset;
} Label;

typedef struct {
    UP2K_Token const* label_token;
    size_t offset;
} LabelPlaceholder;

CREATE_HASHMAP_DECLARATION(LabelMap, UP2K_Address, label_map)
CREATE_HASHMAP_DEFINITION(LabelMap, UP2K_Address, label_map)

CREATE_VECTOR_DECLARATION(ArgumentVector, Argument, argument_vector)
CREATE_VECTOR_DEFINITION(ArgumentVector, Argument, argument_vector)

CREATE_VECTOR_DECLARATION(LabelPlaceholderVector, LabelPlaceholder, label_placeholder_vector)
CREATE_VECTOR_DEFINITION(LabelPlaceholderVector, LabelPlaceholder, label_placeholder_vector)

typedef struct {
    UP2K_TokenVector tokens;
    UP2K_OpcodeList opcodes;
    size_t current;
    UP2K_ByteVector machine_code;
    LabelMap labels;
    UP2K_SourceFile source_file;
    LabelPlaceholderVector label_placeholders;
    UP2K_ConstantsMap const* constants;
    UP2K_InstructionMapVector *UP2K_instruction_map_vector;
} ParserState;

typedef struct {
    uint8_t bytes[4];
} U32Bytes;

typedef struct {
    uint8_t bytes[8];
} U64Bytes;

static ParserState state;

static UP2K_Token* current(void) {
    return &state.tokens.data[state.current];
}

static UP2K_Token* peek(void) {
    return state.current == state.tokens.size - 1 ? NULL : &state.tokens.data[state.current + 1];
}

static UP2K_Token* next(void) {
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
    UP2K_byte_vector_push(&state.machine_code, bytes.bytes[0]);
    UP2K_byte_vector_push(&state.machine_code, bytes.bytes[1]);
    UP2K_byte_vector_push(&state.machine_code, bytes.bytes[2]);
    UP2K_byte_vector_push(&state.machine_code, bytes.bytes[3]);
}

static void emit_u64(uint64_t value) {
    assert(state.machine_code.size % 4 == 0 && "invalid alignment");
    if (state.machine_code.size % 8 != 0) {
        // if there are any labels referencing this code segment => adjust them
        // so that they also have the right alignment
        for (size_t i = 0; i < state.labels.capacity; ++i) {
            if (label_map_is_index_occupied(&state.labels, i)
                && state.labels.data[i].value == state.machine_code.size
            ) {
                if (state.labels.data[i].value == state.machine_code.size) {
                    state.labels.data[i].value += 4;
                }
            }
        }

        // fix alignment by inserting padding
        emit_u32(0xDEADC0DE);
    }
    assert(state.machine_code.size % 8 == 0 && "invalid alignment");
    U64Bytes const bytes = u64_to_big_endian(value);
    UP2K_byte_vector_push(&state.machine_code, bytes.bytes[0]);
    UP2K_byte_vector_push(&state.machine_code, bytes.bytes[1]);
    UP2K_byte_vector_push(&state.machine_code, bytes.bytes[2]);
    UP2K_byte_vector_push(&state.machine_code, bytes.bytes[3]);
    UP2K_byte_vector_push(&state.machine_code, bytes.bytes[4]);
    UP2K_byte_vector_push(&state.machine_code, bytes.bytes[5]);
    UP2K_byte_vector_push(&state.machine_code, bytes.bytes[6]);
    UP2K_byte_vector_push(&state.machine_code, bytes.bytes[7]);
}

static void overwrite_u32(size_t const offset, uint32_t const value) {
    U32Bytes bytes = u32_to_big_endian(value);
    state.machine_code.data[offset + 0] = bytes.bytes[0];
    state.machine_code.data[offset + 1] = bytes.bytes[1];
    state.machine_code.data[offset + 2] = bytes.bytes[2];
    state.machine_code.data[offset + 3] = bytes.bytes[3];
}

static bool register_label(UP2K_StringView label_name) {
    // TODO: this function performes two hashmap-lookups => get rid of one of them!!!!
    UP2K_Address* label = label_map_get(&state.labels, label_name);
    if (label != NULL) {
        // label with this name already exists
        return false;
    }
    label_map_insert(&state.labels, label_name, (UP2K_Address)state.machine_code.size);
    return true;
}

static void UP2K_error_on_token(char const * const message, UP2K_Token const * const token) {
    UP2K_error(
        state.source_file,
        message,
        token->line,
        token->column,
        token->string_view.length
    );
}

static void UP2K_error_on_current_token(char const * const message) {
    UP2K_error_on_token(message, current());
}

static void init_state(
    UP2K_SourceFile const source_file,
    UP2K_TokenVector const tokens,
    UP2K_OpcodeList const opcodes,
    UP2K_ConstantsMap const* constants,
    UP2K_InstructionMapVector *UP2K_instruction_map_vector
) {
    state = (ParserState){
        .tokens = tokens,
        .opcodes = opcodes,
        .current = 0,
        .machine_code = UP2K_byte_vector_create(),
        .labels = label_map_create(0),
        .source_file = source_file,
        .label_placeholders = label_placeholder_vector_create(),
        .constants = constants,
        .UP2K_instruction_map_vector = UP2K_instruction_map_vector,
    };
}

static UP2K_ByteVector cleanup_state(void) {
    label_map_free(&state.labels);
    label_placeholder_vector_free(&state.label_placeholders);
    return state.machine_code;
}

static bool do_arguments_match(UP2K_ArgumentType const lhs, UP2K_ArgumentType const rhs) {
    return lhs == rhs;
}

static bool do_argument_lists_match(
    ArgumentVector const arguments,
    UP2K_OpcodeSpecification const * const UP2K_Opcode
) {
    assert(arguments.size == UP2K_Opcode->argument_count);
    for (size_t i = 0; i < UP2K_Opcode->argument_count; ++i) {
        if (!do_arguments_match(arguments.data[i].type, UP2K_Opcode->required_arguments[i])) {
            return false;
        }
    }
    return true;
}

static UP2K_OpcodeSpecification const* find_opcode(UP2K_Token const * const mnemonic, ArgumentVector const arguments) {
    for (size_t i = 0; i < state.opcodes.num_specifications; ++i) {
        UP2K_OpcodeSpecification const * const UP2K_Opcode = &state.opcodes.specifications[i];
        if (
            UP2K_Opcode->argument_count == arguments.size
            && UP2K_string_view_compare_case_insensitive(mnemonic->string_view, UP2K_Opcode->mnemonic) == 0
            && do_argument_lists_match(arguments, UP2K_Opcode)
        ) {
            return UP2K_Opcode;
        }
    }
    return NULL;
}

static void emit_instruction(UP2K_Token const * const mnemonic, ArgumentVector const arguments) {
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

    UP2K_OpcodeSpecification const * const opcode_specification = find_opcode(mnemonic, arguments);
    if (opcode_specification == NULL) {
        UP2K_error_on_token("unknown instruction or invalid arguments", mnemonic);
    } else {
        fprintf(
            stderr,
            "\tfound matching UP2K_Opcode: %.*s (%#.04x)\n",
            (int)opcode_specification->name.length,
            opcode_specification->name.data,
            opcode_specification->UP2K_Opcode
        );
    }

    UP2K_Instruction instruction = ((UP2K_Instruction)opcode_specification->UP2K_Opcode) << 48;

    bool success;
    UP2K_Word word_result;
    UP2K_Register register_result;

    for (size_t i = 0; i < arguments.size; ++i) {
        switch (arguments.data[i].type) {
            case UP2K_ARGUMENT_TYPE_ADDRESS_POINTER:
                assert(arguments.data[i].first_token->type == TOKEN_TYPE_ASTERISK);
                /* The argument can either be an address (e.g. *0x10) or a label
                 * (e.g. *main) or a constant (e.g. *TERMINAL_START). In the second
                 * and third case we do not do anything since the address will be
                 * inserted later. Otherwise, we UP2K_parse the number and use it as an
                 * address. */
                assert(
                    (arguments.data[i].first_token + 1)->type == TOKEN_TYPE_IDENTIFIER
                    || (arguments.data[i].first_token + 1)->type == TOKEN_TYPE_WORD_LITERAL
                );
                if ((arguments.data[i].first_token + 1)->type == TOKEN_TYPE_IDENTIFIER) {
                    size_t const offset = state.machine_code.size
                        + (state.machine_code.size % 8 == 0 ? 4 : 8);
                    label_placeholder_vector_push(
                        &state.label_placeholders,
                        (LabelPlaceholder){
                            .label_token = arguments.data[i].first_token + 1,
                            .offset = offset,
                        }
                    );
                    instruction |= 0xDEADC0DE; // placeholder
                } else {
                    UP2K_word_from_token(arguments.data[i].first_token + 1, &success, &word_result);
                    fprintf(
                        stderr,
                        "%.*s\n",
                        (int)(arguments.data[i].first_token + 1)->string_view.length,
                        (arguments.data[i].first_token + 1)->string_view.data
                    );
                    if (!success) {
                        UP2K_error_on_token("invalid pointer value", arguments.data[i].first_token + 1);
                    }
                    instruction |= word_result;
                }
                break;
            case UP2K_ARGUMENT_TYPE_IMMEDIATE: {
                UP2K_TokenType const token_type = arguments.data[i].first_token->type;
                assert(
                    token_type == TOKEN_TYPE_IDENTIFIER
                    || token_type == TOKEN_TYPE_WORD_LITERAL
                    || token_type == TOKEN_TYPE_WORD_CONSTANT
                );
                if (token_type == TOKEN_TYPE_IDENTIFIER) {
                    // label
                    size_t const offset = state.machine_code.size
                        + (state.machine_code.size % 8 == 0 ? 4 : 8);
                    label_placeholder_vector_push(
                        &state.label_placeholders,
                        (LabelPlaceholder){
                            .label_token = arguments.data[i].first_token,
                            .offset = offset,
                        }
                    );
                    instruction |= 0xDEADC0DE; // placeholder
                } else if (token_type == TOKEN_TYPE_WORD_LITERAL) {
                    // immediate
                    UP2K_word_from_token(arguments.data[i].first_token, &success, &word_result);
                    if (!success) {
                        UP2K_error_on_token("invalid immediate value", arguments.data[i].first_token);
                    }
                    instruction |= word_result;
                } else if (token_type == TOKEN_TYPE_WORD_CONSTANT) {
                    bool constant_found;
                    uint64_t constant_value;
                    UP2K_get_constant_value(
                        arguments.data[i].first_token->string_view,
                        CONSTANT_TYPE_UNSIGNED_INTEGER,
                        &constant_found,
                        &constant_value,
                        state.constants);
                    assert(constant_found && "look-up happened before and therefore now should be found");
                    fprintf(stderr, "\treplaced numeric constant with value %"PRIu64"\n", constant_value);
                    instruction |= (UP2K_Instruction)constant_value;
                } else {
                    assert(false && "unreachable");
                }
                break;
            }
            case UP2K_ARGUMENT_TYPE_NONE:
                assert(false && "unreachable");
                break;
            case UP2K_ARGUMENT_TYPE_REGISTER_POINTER: {
                /* the argument can now either be a register point (e.g. "*R6") or a register constant
                 * pointer (e.g. "*sp" for dereferencing the stack pointer) */
                assert(arguments.data[i].first_token->type == TOKEN_TYPE_ASTERISK);
                UP2K_TokenType const token_type = (arguments.data[i].first_token + 1)->type;
                if (token_type == TOKEN_TYPE_REGISTER) {
                    UP2K_register_from_token(arguments.data[i].first_token + 1, &success, &register_result);
                    if (!success) {
                        UP2K_error_on_token("invalid register identifier", arguments.data[i].first_token + 1);
                    }
                    instruction |= ((UP2K_Instruction)register_result) << opcode_specification->offsets[i];
                } else if (token_type == TOKEN_TYPE_REGISTER_CONSTANT) {
                    bool constant_found;
                    uint64_t constant_value;
                    UP2K_get_constant_value(
                        (arguments.data[i].first_token + 1)->string_view,
                        CONSTANT_TYPE_REGISTER,
                        &constant_found,
                        &constant_value,
                        state.constants);
                    assert(constant_found && "look-up happened before and thefeore should not fail here");
                    fprintf(stderr, "\treplaced register constant with a value of %"PRIu64"\n", constant_value);
                    instruction |= ((UP2K_Instruction)constant_value) << opcode_specification->offsets[i];
                } else {
                    assert(false && "unreachable");
                }
                break;
            }
            case UP2K_ARGUMENT_TYPE_REGISTER: {
                /* the argument can now either be a register (e.g. R6) or a register constant
                 * (e.g. "sp" for the stack pointer) */
                UP2K_TokenType const token_type = arguments.data[i].first_token->type;
                if (token_type == TOKEN_TYPE_REGISTER) {
                    UP2K_register_from_token(arguments.data[i].first_token, &success, &register_result);
                    if (!success) {
                        UP2K_error_on_token("invalid register identifier", arguments.data[i].first_token);
                    }
                    instruction |= ((UP2K_Instruction)register_result) << opcode_specification->offsets[i];
                } else if (token_type == TOKEN_TYPE_REGISTER_CONSTANT) {
                    bool constant_found;
                    uint64_t constant_value;
                    UP2K_get_constant_value(
                        arguments.data[i].first_token->string_view,
                        CONSTANT_TYPE_REGISTER,
                        &constant_found,
                        &constant_value,
                        state.constants);
                    assert(constant_found && "look-up happened before and thefeore should not fail here");
                    fprintf(stderr, "\treplaced register constant with a value of %"PRIu64"\n", constant_value);
                    instruction |= ((UP2K_Instruction)constant_value) << opcode_specification->offsets[i];
                } else {
                    assert(false && "unreachable");
                }
                break;
            }
        }
    }

    emit_u64(instruction);
}

static void parse_label(void) {
    assert(peek()->type == TOKEN_TYPE_COLON);
    if (!register_label(UP2K_identifier_from_token(current()))) {
        UP2K_error_on_current_token("label redefinition");
    }
    next(); // proceed to colon token
    if (peek()->type != TOKEN_TYPE_NEWLINE) {
        UP2K_error_on_current_token("newline expected after label definition");
    }
}

static void parse_instruction(void) {
    UP2K_Token const * const mnemonic = current();
    UP2K_Token const* current_argument_start = NULL;
    ArgumentVector arguments = argument_vector_create();
    bool valid_argument_start_position = true;
    next();
    while (true) {
        if (current()->type == TOKEN_TYPE_COMMA) {
            if (valid_argument_start_position) {
                UP2K_error_on_current_token("unexpected comma");
            } else {
                next();
                valid_argument_start_position = true;
            }
        }
        if (current()->type == TOKEN_TYPE_ASTERISK) {
            if (!valid_argument_start_position) {
                UP2K_error_on_current_token("comma expected");
            }
            // pointer or address or label pointer
            current_argument_start = current();
        } else if (current()->type == TOKEN_TYPE_EOF) {
            UP2K_error(state.source_file, "unexpected end of file", current()->line, current()->column, 1);
        } else if (current()->type == TOKEN_TYPE_NEWLINE) {
            if (current_argument_start != NULL) {
                UP2K_error_on_current_token("register or address expected");
            }
            break;
        } else {
            if (current_argument_start == NULL) {
                // no pointer and no address
                if (!valid_argument_start_position) {
                    UP2K_error_on_current_token("comma expected");
                }
                switch (current()->type) {
                    case TOKEN_TYPE_WORD_CONSTANT:
                    case TOKEN_TYPE_WORD_LITERAL:
                        argument_vector_push(&arguments, (Argument){
                            .type = UP2K_ARGUMENT_TYPE_IMMEDIATE,
                            .first_token = current(),
                        });
                        valid_argument_start_position = false;
                        break;
                    case TOKEN_TYPE_REGISTER_CONSTANT:
                    case TOKEN_TYPE_REGISTER:
                        argument_vector_push(&arguments, (Argument){
                            .type = UP2K_ARGUMENT_TYPE_REGISTER,
                            .first_token = current(),
                        });
                        valid_argument_start_position = false;
                        break;
                    case TOKEN_TYPE_IDENTIFIER:
                        argument_vector_push(&arguments, (Argument){
                            .type = UP2K_ARGUMENT_TYPE_IMMEDIATE,
                            .first_token = current(),
                        });
                        valid_argument_start_position = false;
                        break;
                    default:
                        UP2K_error_on_current_token("invalid argument");
                }
            } else {
                // second token of pointer or address or label pointer
                switch (current()->type) {
                    case TOKEN_TYPE_WORD_CONSTANT:
                    case TOKEN_TYPE_WORD_LITERAL:
                        argument_vector_push(&arguments, (Argument){
                            .type = UP2K_ARGUMENT_TYPE_ADDRESS_POINTER,
                            .first_token = current_argument_start,
                        });
                        valid_argument_start_position = false;
                        break;
                    case TOKEN_TYPE_REGISTER_CONSTANT:
                    case TOKEN_TYPE_REGISTER:
                        argument_vector_push(&arguments, (Argument){
                            .type = UP2K_ARGUMENT_TYPE_REGISTER_POINTER,
                            .first_token = current_argument_start,
                        });
                        valid_argument_start_position = false;
                        break;
                    case TOKEN_TYPE_IDENTIFIER:
                        argument_vector_push(&arguments, (Argument){
                            .type = UP2K_ARGUMENT_TYPE_ADDRESS_POINTER,
                            .first_token = current_argument_start,
                        });
                        valid_argument_start_position = false;
                        break;
                    default:
                        UP2K_error_on_current_token("invalid argument");
                }
                current_argument_start = NULL;
            }
        }
        next();
    }

    emit_instruction(mnemonic, arguments);
    argument_vector_free(&arguments);

    // Writing line mapping after emitting instruction, because
    // emit_instruction can increase position by 8 (normal) or
    // by 12 (to fix alignment).
    if (state.UP2K_instruction_map_vector != NULL) {
        UP2K_instruction_map_vector_push(state.UP2K_instruction_map_vector, (UP2K_InstructionMap){
            .line = mnemonic->line,
            .address = state.machine_code.size - 8,
        });
    }
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
        UP2K_error_on_current_token("expected UP2K_Word literal");
    }
    bool success;
    UP2K_Word result;
    UP2K_word_from_token(current(), &success, &result);
    assert(success); // lexer should only UP2K_tokenize UP2K_Word literals if they are valid
    emit_u32(result);
    fprintf(stderr, "should emit UP2K_Word literal: %"PRIu32"\n", result);
    next();
}

static void parse_words_literal(void) {
    assert(current()->type == TOKEN_TYPE_IDENTIFIER);
    assert(UP2K_string_view_compare_case_insensitive(current()->string_view, UP2K_string_view_from_string("words")) == 0);
    next();
    if (current()->type != TOKEN_TYPE_LEFT_BRACKET) {
        UP2K_error_on_current_token("\"[\" expected");
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
        UP2K_error_on_current_token("\"]\" expected");
    }
    next();
    if (current()->type != TOKEN_TYPE_NEWLINE) {
        UP2K_error_on_current_token("newline expected after words literal");
    }
}

static void emit_quoted_escaped_string(UP2K_StringView string) {
    size_t const length_byte_offset = state.machine_code.size;
    emit_u32(0); // length byte, will be replaced later
    char const* current = string.data + 1;
    char const * const end = string.data + string.length - 1;
    UP2K_Word length = 0;
    while (current != end) {
        switch (*current) {
            case '\\':
                ++current;
                assert(current != end); // the lexer already checks for valid escaping
                switch (*current) {
                    case '"':
                        emit_u32((UP2K_Word)'"');
                        break;
                    case '\\':
                        emit_u32((UP2K_Word)'\\');
                        break;
                    case 't':
                        emit_u32((UP2K_Word)'\t');
                        break;
                    case 'n':
                        emit_u32((UP2K_Word)'\n');
                        break;
                    case 'v':
                        emit_u32((UP2K_Word)'\v');
                        break;
                    case 'f':
                        emit_u32((UP2K_Word)'\f');
                        break;
                    case 'r':
                        emit_u32((UP2K_Word)'\r');
                        break;
                    default:
                        assert(false && "invalid escape sequence"); // should be caught by the lexer
                        break;
                }
                break;
            default:
                emit_u32((UP2K_Word)*current);
                break;
        }
        ++length;
        ++current;
    }
    overwrite_u32(length_byte_offset, length);
}

static void parse_string_literal(void) {
    assert(current()->type == TOKEN_TYPE_IDENTIFIER);
    assert(UP2K_string_view_compare_case_insensitive(current()->string_view, UP2K_string_view_from_string("string")) == 0);
    next();
    if (current()->type != TOKEN_TYPE_STRING_LITERAL) {
        UP2K_error_on_current_token("string literal expected");
    }
    fprintf(stderr, "should emit string literal: %.*s\n", (int)current()->string_view.length, current()->string_view.data);
    emit_quoted_escaped_string(current()->string_view);
    next();
    if (current()->type != TOKEN_TYPE_NEWLINE) {
        UP2K_error_on_current_token("newline expected after string literal");
    }
}

static void parse_literal(void) {
    assert(current()->type == TOKEN_TYPE_DOT);
    next();
    if (current()->type != TOKEN_TYPE_IDENTIFIER) {
        UP2K_error_on_current_token("expected literal");
    }

    if (UP2K_string_view_compare_case_insensitive(current()->string_view, UP2K_string_view_from_string("words")) == 0) {
        parse_words_literal();
    } else if (UP2K_string_view_compare_case_insensitive(current()->string_view, UP2K_string_view_from_string("string")) == 0) {
        parse_string_literal();
    } else {
        UP2K_error_on_current_token("expected either \"words\" or \"string\"");
    }
}

UP2K_ByteVector UP2K_parse(
    UP2K_SourceFile const source_file,
    UP2K_TokenVector const tokens,
    UP2K_OpcodeList const opcodes,
    UP2K_ConstantsMap const* constants,
    UP2K_InstructionMapVector *UP2K_instruction_map_vector
) {
    assert(tokens.size > 0);
    assert(tokens.data[tokens.size - 1].type == TOKEN_TYPE_EOF);
    if (tokens.size == 1) {
        UP2K_error(source_file, "empty input", tokens.data[0].line, tokens.data[0].column, 1);
    }
    init_state(source_file, tokens, opcodes, constants, UP2K_instruction_map_vector);
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
                UP2K_error_on_current_token("unexpected token");
                break;
        }
        next();
    }
    for (size_t i = 0; i < state.labels.capacity; ++i) {
        if (label_map_is_index_occupied(&state.labels, i)) {
            fprintf(
                stderr,
                "%.*s @ 0x%08"PRIX32"\n",
                (int)state.labels.data[i].key.length,
                state.labels.data[i].key.data,
                state.labels.data[i].value
            );
        }
    }

    bool entry_point_found;
    uint64_t entry_point;
    UP2K_get_constant_value(
        UP2K_string_view_from_string("ENTRY_POINT"),
        CONSTANT_TYPE_ADDRESS,
        &entry_point_found,
        &entry_point,
        state.constants);
    assert(entry_point && "entry point must be found");

    if (UP2K_instruction_map_vector != NULL) {
        for (size_t i = 0; i < UP2K_instruction_map_vector->size; ++i) {
            UP2K_instruction_map_vector->data[i].address += entry_point;
        }
    }

    for (size_t i = 0; i < state.label_placeholders.size; ++i) {
        LabelPlaceholder const * const placeholder = &state.label_placeholders.data[i];

        bool found_constant;
        uint64_t constant_value;
        UP2K_get_constant_value(
            placeholder->label_token->string_view,
            CONSTANT_TYPE_ADDRESS,
            &found_constant,
            &constant_value,
            state.constants);
        if (found_constant) {
            fprintf(
                stderr,
                "Replacing value at %zu with constant %"PRIx64"\n",
                placeholder->offset,
                constant_value
            );
            overwrite_u32(placeholder->offset, (UP2K_Word)constant_value);
            continue;
        }

        UP2K_Address* offset = label_map_get(&state.labels, placeholder->label_token->string_view);
        if (offset == NULL) {
            UP2K_error_on_token("unknown label", placeholder->label_token);
        } else {
            assert(state.machine_code.size > placeholder->offset && "invalid offset");
            fprintf(
                stderr,
                "Replacing label at %zu with %"PRIx32"\n",
                placeholder->offset,
                *offset + (UP2K_Address)entry_point
            );
            overwrite_u32(placeholder->offset, *offset + (UP2K_Address)entry_point);
        }
    }

    return cleanup_state();
}
