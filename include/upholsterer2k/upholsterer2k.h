#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "upholsterer2k/string_view.h"

StringView get_text() {
    return string_view_from_string("Test");
}

#ifdef __cplusplus
}
#endif
