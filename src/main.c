#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include "upholsterer2k/lexer.h"
#include "upholsterer2k/string_view.h"
#include "upholsterer2k/source_file.h"
#include "upholsterer2k/opcodes.h"
#include "upholsterer2k/parser.h"
#include "upholsterer2k/upholsterer2k.h"

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

typedef struct {
    bool valid;
    char const* source_file_name;
    char const* instruction_map_file_name;
} Arguments;

void read_whole_file(FILE* const file, char** contents, size_t* length) {
    size_t capacity = 0;
    size_t size = 0;
    char* buffer = NULL;
    int current_char;
    while ((current_char = fgetc(file)) != EOF) {
        if (size == capacity) {// needs to resize
            capacity = capacity == 0 ? 1 : 2 * capacity;
            char* const new_buffer = realloc(buffer, capacity);
            if (new_buffer == NULL) {
                free(buffer);
                *contents = NULL;
                *length = 0;
                return;
            }
            buffer = new_buffer;
        }
        buffer[size] = (char) current_char;
        ++size;
    }
    *contents = buffer;
    *length = size;
}

void write_instruction_map(UP2K_InstructionMapVector UP2K_instruction_map_vector, char const* const file_name) {
    FILE* file = fopen(file_name, "w");
    if (!file) {
        fprintf(stderr, "Could not open file %s to write instruction mappings to: %s.\n", file_name, strerror(errno));
        return;
    }

    fprintf(file, "line address\n");
    for (size_t i = 0; i < UP2K_instruction_map_vector.size; ++i) {
        UP2K_InstructionMap map = UP2K_instruction_map_vector.data[i];
        fprintf(file, "%zu %zu\n", map.line, map.address);
    }

    fclose(file);
}

bool string_starts_with(char const* const string, char const* const prefix) {
    return strncmp(prefix, string, strlen(prefix)) == 0;
}

Arguments parse_arguments(int argc, char** argv) {
    Arguments arguments = {
        .valid = true,
        .source_file_name = NULL,
        .instruction_map_file_name = NULL,
    };

    for (int i = 1; i < argc; ++i) {
        if (string_starts_with(argv[i], "-m")) {
            const size_t key_length = strlen("-m");
            if (strlen(argv[i]) > key_length) {
                // Case "-mfile"
                arguments.instruction_map_file_name = argv[i] + key_length;
            } else if (i + 1 < argc) {
                // Case "-m file"
                ++i;
                arguments.instruction_map_file_name = argv[i];
            } else {
                fprintf(stderr, "UP2K_error: Switch 'm'/'map' requires a value.\n");
                arguments.valid = false;
                return arguments;
            }
        } else if (string_starts_with(argv[i], "--map")) {
            const size_t key_length = strlen("--map");
            const size_t argument_length = strlen(argv[i]);
            if (argument_length > key_length + 1 && argv[i][key_length] == '=') {
                // Case "--map=file"
                arguments.instruction_map_file_name = argv[i] + key_length + 1;
            } else if (argument_length == key_length && i + 1 < argc) {
                // Case "--map file"
                ++i;
                arguments.instruction_map_file_name = argv[i];
            } else {
                fprintf(stderr, "UP2K_error: Switch 'm'/'map' requires a value.\n");
                arguments.valid = false;
                return arguments;
            }
        } else {
            // Positional Argument(s)
            if (arguments.source_file_name == NULL) {
                arguments.source_file_name = argv[i];
            } else {
                fprintf(stderr, "UP2K_error: Too many arguments.\n");
                arguments.valid = false;
                return arguments;
            }
        }
    }

    return arguments;
}

int main(int argc, char** argv) {
    Arguments arguments = parse_arguments(argc, argv);
    if (!arguments.valid) {
        fprintf(stderr, "Usage: %s [UP2K_SourceFile]\n", argv[0]);
        return EXIT_FAILURE;
    }

    char* source_data = NULL;
    UP2K_StringView source = { 0 };

    if (arguments.source_file_name == NULL) {
        size_t length;
        read_whole_file(stdin, &source_data, &length);
        source = (UP2K_StringView){ .data = source_data, .length = length };
    } else {
        FILE* file = fopen(arguments.source_file_name, "r");
        if (!file) {
            fprintf(stderr, "Could not open file %s: %s.\n", arguments.source_file_name, strerror(errno));
            return EXIT_FAILURE;
        }
        size_t length;
        read_whole_file(file, &source_data, &length);
        fclose(file);
        source = (UP2K_StringView){ .data = source_data, .length = length };
    }
    UP2K_SourceFile source_file = {
        .filename =
                UP2K_string_view_from_string(arguments.source_file_name == NULL ? "<stdin>" : arguments.source_file_name),
        .source = source,
    };

    UP2K_ByteVector machine_code;
    UP2K_InstructionMapVector UP2K_instruction_map_vector = UP2K_instruction_map_vector_create();
    char UP2K_error_message[512];
    size_t const buffer_size = sizeof(UP2K_error_message) / sizeof(UP2K_error_message[0]);
    bool const result = UP2K_bssemble(source_file, arguments.instruction_map_file_name, &machine_code,
                                 &UP2K_instruction_map_vector, UP2K_error_message, buffer_size);

    if (!result) {
        fprintf(stderr, "%s", UP2K_error_message);
    } else {
        // when in windows, we have to set the mode of stdout to binary because otherwise
        // every \n will be automatically replaced with \r\n which destroys the generated
        // binary
#ifdef _WIN32
        _setmode(_fileno(stdout), _O_BINARY);
#endif

        UP2K_write_machine_code(machine_code, stdout);
        if (arguments.instruction_map_file_name != NULL) {
            write_instruction_map(UP2K_instruction_map_vector, arguments.instruction_map_file_name);
        }
    }

    // cleanup
    UP2K_instruction_map_vector_free(&UP2K_instruction_map_vector);
    UP2K_byte_vector_free(&machine_code);
    free(source_data);
    return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
