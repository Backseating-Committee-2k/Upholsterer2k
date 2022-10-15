#pragma once

#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char const * data;
    size_t length;
} StringView;

StringView string_view_from_pointers(char const * start_inclusive, char const* end_exclusive);
StringView string_view_from_string(char const* string);
int string_view_compare(StringView lhs, StringView rhs);
int string_view_compare_case_insensitive(StringView lhs, StringView rhs);
void print_string_view(FILE* stream, StringView string_view);
void string_view_stderr(StringView string_view);
void string_view_stdout(StringView string_view);
