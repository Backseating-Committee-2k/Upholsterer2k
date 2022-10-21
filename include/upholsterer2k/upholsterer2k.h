#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "source_file.h"
#include "parser.h"
#include <stdbool.h>
#include <stdlib.h>

void UP2K_write_machine_code(UP2K_ByteVector machine_code, FILE* const file);

bool UP2K_bssemble(UP2K_SourceFile source_file,
              char const* instruction_map_file_name,
              UP2K_ByteVector* out_machine_code,
              UP2K_InstructionMapVector* out_instruction_map,
              char* out_error_message_buffer,
              size_t buffer_size);

#ifdef __cplusplus
}
#endif
