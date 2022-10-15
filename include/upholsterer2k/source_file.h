#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "upholsterer2k/string_view.h"

typedef struct {
    StringView filename;
    StringView source;
} SourceFile;

#ifdef __cplusplus
}
#endif
