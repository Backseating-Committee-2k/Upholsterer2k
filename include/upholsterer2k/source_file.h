#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "upholsterer2k/string_view.h"

typedef struct {
    UP2K_StringView filename;
    UP2K_StringView source;
} UP2K_SourceFile;

#ifdef __cplusplus
}
#endif
