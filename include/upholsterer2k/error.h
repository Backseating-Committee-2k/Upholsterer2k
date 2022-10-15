#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "upholsterer2k/source_file.h"
#include <stdlib.h>

void error(SourceFile source_file, char const* message, size_t line, size_t column, size_t squiggly_length);

#ifdef __cplusplus
}
#endif
