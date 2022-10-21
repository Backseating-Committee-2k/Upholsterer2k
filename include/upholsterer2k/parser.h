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

CREATE_VECTOR_DECLARATION(UP2K_ByteVector, uint8_t, UP2K_byte_vector)

/// Maps the instruction identified by the
/// line number to an address in the output binary.
typedef struct {
    size_t line;
    size_t address;
} UP2K_InstructionMap;

CREATE_VECTOR_DECLARATION(UP2K_InstructionMapVector, UP2K_InstructionMap, UP2K_instruction_map_vector)

UP2K_ByteVector UP2K_parse(UP2K_SourceFile source_file,
                 UP2K_TokenVector tokens,
                 UP2K_OpcodeList opcodes,
                 UP2K_ConstantsMap const* constants,
                 UP2K_InstructionMapVector* UP2K_instruction_map_vector);

#ifdef __cplusplus
}
#endif
