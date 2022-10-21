#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "upholsterer2k/string_view.h"
#include "upholsterer2k/token_vector.h"
#include "upholsterer2k/source_file.h"
#include "upholsterer2k/constants.h"

UP2K_TokenVector UP2K_tokenize(UP2K_SourceFile source_file, UP2K_ConstantsMap const* constants);

#ifdef __cplusplus
}
#endif
