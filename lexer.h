#pragma once

#include "string_view.h"
#include "token_vector.h"
#include "source_file.h"

TokenVector tokenize(SourceFile source_file);
