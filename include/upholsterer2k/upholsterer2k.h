#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "source_file.h"
#include "parser.h"
#include <stdbool.h>
#include <stdlib.h>

bool bssemble(SourceFile source_file,
              char const* instruction_map_file_name,
              ByteVector* out_machine_code,
              InstructionMapVector* out_instruction_map,
              char* out_error_message_buffer,
              size_t buffer_size);

#ifdef __cplusplus
}
#endif
