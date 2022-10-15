#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "upholsterer2k/vector.h"
#include "upholsterer2k/token_vector.h"
#include "upholsterer2k/opcodes.h"
#include "upholsterer2k/source_file.h"
#include "upholsterer2k/constants.h"

CREATE_VECTOR_DECLARATION(ByteVector, uint8_t, byte_vector)

/// Maps the instruction identified by the
/// line number to an address in the output binary.
typedef struct {
    size_t line;
    size_t address;
} InstructionMap;

CREATE_VECTOR_DECLARATION(InstructionMapVector, InstructionMap, instruction_map_vector)

ByteVector parse(SourceFile source_file,
                 TokenVector tokens,
                 OpcodeList opcodes,
                 ConstantsMap const* constants,
                 InstructionMapVector* instruction_map_vector);

#ifdef __cplusplus
}
#endif
