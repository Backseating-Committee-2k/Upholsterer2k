#pragma once

#include <stdint.h>
#include "vector.h"
#include "token_vector.h"
#include "opcodes.h"
#include "source_file.h"
#include "constants.h"

CREATE_VECTOR_DECLARATION(ByteVector, uint8_t, byte_vector)

/// Maps the instruction identified by the
/// line number to an address in the output binary.
typedef struct {
    size_t line;
    size_t address;
} InstructionMap;

CREATE_VECTOR_DECLARATION(InstructionMapVector, InstructionMap, instruction_map_vector)

ByteVector parse(SourceFile source_file, TokenVector tokens, OpcodeList opcodes, ConstantsMap const* constants, InstructionMapVector *instruction_map_vector);
