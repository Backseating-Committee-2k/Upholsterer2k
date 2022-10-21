#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char const* data;
    size_t length;
} UP2K_StringView;

UP2K_StringView UP2K_string_view_from_pointers(char const* start_inclusive, char const* end_exclusive);
UP2K_StringView UP2K_string_view_from_string(char const* string);
int UP2K_string_view_compare(UP2K_StringView lhs, UP2K_StringView rhs);
int UP2K_string_view_compare_case_insensitive(UP2K_StringView lhs, UP2K_StringView rhs);
void UP2K_print_string_view(FILE* stream, UP2K_StringView string_view);
void UP2K_string_view_stderr(UP2K_StringView string_view);
void UP2K_string_view_stdout(UP2K_StringView string_view);

#ifdef __cplusplus
}
#endif
