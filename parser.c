#include "parser.h"
#include "error.h"
#include "types.h"
#include "constants.h"
#include <stdlib.h>
#include <assert.h>

CREATE_VECTOR_DEFINITION(ByteVector, uint8_t, byte_vector)

typedef struct {
    StringView name;
    Address address;
} Label;

CREATE_VECTOR_DECLARATION(LabelVector, Label, label_vector)
CREATE_VECTOR_DEFINITION(LabelVector, Label, label_vector)

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

Token* current() {
    return &state.tokens.data[state.current];
}

Token* peek() {
    return state.current == state.tokens.size - 1 ? NULL : &state.tokens.data[state.current + 1];
}

Token* next() {
    if (state.current == state.tokens.size - 1) {
        return NULL;
    }
    ++state.current;
    return current();
}

U32Bytes u32_to_big_endian(uint32_t value) {
    return (U32Bytes){
        .bytes = {
            (uint8_t)(value >> 24),
            (uint8_t)(value >> 16),
            (uint8_t)(value >> 8),
            (uint8_t)(value >> 0),
        }
    };
}

U64Bytes u64_to_big_endian(uint64_t value) {
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

void emit_u32(uint32_t value) {
    U32Bytes const bytes = u32_to_big_endian(value);
    byte_vector_push(&state.machine_code, bytes.bytes[0]);
    byte_vector_push(&state.machine_code, bytes.bytes[1]);
    byte_vector_push(&state.machine_code, bytes.bytes[2]);
    byte_vector_push(&state.machine_code, bytes.bytes[3]);
}

void emit_u64(uint64_t value) {
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

bool register_label(StringView label_name) {
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

void error_on_current_token(char const * const message) {
    error(
        state.source_file,
        message,
        current()->line,
        current()->column,
        current()->string_view.length
    );
}

void init_state(SourceFile const source_file, TokenVector const tokens, OpcodeList const opcodes) {
    state = (ParserState){
        .tokens = tokens,
        .opcodes = opcodes,
        .current = 0,
        .machine_code = byte_vector_create(),
        .labels = label_vector_create(),
        .source_file = source_file,
    };
}

ByteVector cleanup_state() {
    label_vector_free(&state.labels);
    return state.machine_code;
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
                if (peek()->type == TOKEN_TYPE_COLON) {
                    // label
                    if (!register_label(identifier_from_token(current()))) {
                        error_on_current_token("label redefinition");
                    }
                } else {
                    // instruction

                }
                break;
        }
        next();
    }
    return cleanup_state();
}
