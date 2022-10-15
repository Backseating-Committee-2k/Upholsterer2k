#pragma once

#include "string_view.h"

typedef struct {
    StringView filename;
    StringView source;
} SourceFile;
