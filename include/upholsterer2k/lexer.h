#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "upholsterer2k/string_view.h"
#include "upholsterer2k/token_vector.h"
#include "upholsterer2k/source_file.h"
#include "upholsterer2k/constants.h"

TokenVector tokenize(SourceFile source_file, ConstantsMap const* constants);

#ifdef __cplusplus
}
#endif
