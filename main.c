#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "lexer.h"
#include "string_view.h"
#include "source_file.h"
#include "opcodes.h"

StringView read_whole_file(FILE* const file) {
    size_t capacity = 0;
    size_t size = 0;
    char* buffer = NULL;
    int current_char;
    while ((current_char = fgetc(file)) != EOF) {
        if (size == capacity) { // needs to resize
            capacity = capacity == 0 ? 1 : 2 * capacity;
            char* const new_buffer = realloc(buffer, capacity);
            if (new_buffer == NULL) {
                free(buffer);
                return (StringView){
                    .data = NULL,
                    .length = 0,
                };
            }
            buffer = new_buffer;
        }
        buffer[size] = (char)current_char;
        ++size;
    }
    return (StringView){
        .data = buffer,
        .length = size,
    };
}

int main(int argc, char** argv) {
    StringView source = { 0 };
    if (argc == 1) {
        source = read_whole_file(stdin);
    } else if (argc == 2) {
        FILE* file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Could not open file %s: %s.\n", argv[1], strerror(errno));
            return EXIT_FAILURE;
        }
        source = read_whole_file(file);
        fclose(file);
    } else {
        fprintf(stderr, "Usage: %s [SOURCEFILE]\n", argv[0]);
        return EXIT_FAILURE;
    }
    SourceFile source_file = {
        .filename = string_view_from_string(argc == 1 ? "<stdin>" : argv[1]),
        .source = source,
    };

    TokenVector tokens = tokenize(source_file);

    for (size_t i = 0; i < tokens.size; ++i) {
        printf("%s", token_type_to_string(tokens.data[i].type));
        switch (tokens.data[i].type) {
            case TOKEN_TYPE_WORD_LITERAL:
            case TOKEN_TYPE_REGISTER:
            case TOKEN_TYPE_IDENTIFIER:
            case TOKEN_TYPE_STRING_LITERAL:
                printf(" (%.*s)", (int)tokens.data[i].string_view.length, tokens.data[i].string_view.data);
                break;
            default:
                break;
        }
        printf("\n");
    }

    token_vector_free(&tokens);
    free(source.data);

    OpcodeList opcodes = opcode_specifications();
    for (size_t i = 0; i < opcodes.num_specifications; ++i) {
        OpcodeSpecification const * const specification = &opcodes.specifications[i];
        printf(
            "%.*s (0x%04lX) -> %.*s\n",
            (int)specification->name.length,
            specification->name.data,
            specification->opcode,
            (int)specification->mnemonic.length,
            specification->mnemonic.data
        );
    }
    free(opcodes.specifications);
    return EXIT_SUCCESS;
}
