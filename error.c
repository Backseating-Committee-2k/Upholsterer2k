#include "upholsterer2k/error.h"
#include <stdio.h>
#include <assert.h>

void UP2K_error(
    UP2K_SourceFile const source_file,
    char const * const message,
    size_t const line,
    size_t const column,
    size_t squiggly_length
) {
    // TODO: replace with snprintf (see https://en.cppreference.com/w/c/io/fprintf)
    fprintf(
        stderr, "%.*s:%zu:%zu: %s\n",
        (int)source_file.filename.length,
        source_file.filename.data,
        line, column, message
    );
    size_t line_start = 0;
    size_t current_line = 1;
    while (current_line < line) {
        assert(line_start < source_file.source.length);
        if (source_file.source.data[line_start] == '\n') {
            ++current_line;
        }
        ++line_start;
    }
    size_t line_end = line_start;
    while (line_end < source_file.source.length && source_file.source.data[line_end] != '\n') {
        ++line_end;
    }
    size_t const line_length = line_end - line_start;
    fprintf(stderr, "%.*s\n", (int)line_length, &source_file.source.data[line_start]);
    fprintf(stderr, "%*c", (int)column, '^');
    for (size_t i = 1; i < squiggly_length; ++i) {
        fputc('~', stderr);
    }
    fputs(" UP2K_error occurred here\n", stderr);
    exit(EXIT_FAILURE); // no memory cleanup? ¯\_(ツ)_/¯
}
