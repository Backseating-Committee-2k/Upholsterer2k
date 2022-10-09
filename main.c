#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include "lexer.h"
#include "string_view.h"
#include "source_file.h"
#include "opcodes.h"
#include "parser.h"

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

typedef struct {
    bool valid;
    char const * source_file_name;
    char const * instruction_map_file_name;
} Arguments;

void read_whole_file(FILE* const file, char** contents, size_t* length) {
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
                *contents = NULL;
                *length = 0;
                return;
            }
            buffer = new_buffer;
        }
        buffer[size] = (char)current_char;
        ++size;
    }
    *contents = buffer;
    *length = size;
}

void write_machine_code(ByteVector machine_code, FILE* const file) {
    fwrite(machine_code.data, sizeof(uint8_t), machine_code.size, file);
}

void check_opcodes(OpcodeList const opcodes) {
    for (size_t i = 0; i < opcodes.num_specifications; ++i) {
        OpcodeSpecification const * const specification = &opcodes.specifications[i];
        assert(specification->mnemonic.length > 0 && "unknown opcode");
    }
}

void write_instruction_map(InstructionMapVector instruction_map_vector, char const * const file_name) {
    FILE* file = fopen(file_name, "w");
    if (!file) {
        fprintf(stderr, "Could not open file %s to write instruction mappings to: %s.\n", file_name, strerror(errno));
        return;
    }

    fprintf(file, "line address\n");
    for (size_t i = 0; i < instruction_map_vector.size; ++i) {
        InstructionMap map = instruction_map_vector.data[i];
        fprintf(file, "%zu %zu\n", map.line, map.address);
    }

    fclose(file);
}

bool string_starts_with(char const * const string, char const * const prefix) {
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
                fprintf(stderr, "Error: Switch 'm'/'map' requires a value.\n");
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
                fprintf(stderr, "Error: Switch 'm'/'map' requires a value.\n");
                arguments.valid = false;
                return arguments;
            }
        } else {
            // Positional Argument(s)
            if (arguments.source_file_name == NULL) {
                arguments.source_file_name = argv[i];
            } else {
                fprintf(stderr, "Error: Too many arguments.\n");
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
        fprintf(stderr, "Usage: %s [SOURCEFILE]\n", argv[0]);
        return EXIT_FAILURE;
    }

    char* source_data = NULL;
    StringView source = { 0 };

    if (arguments.source_file_name == NULL) {
        size_t length;
        read_whole_file(stdin, &source_data, &length);
        source = (StringView){ .data = source_data, .length = length };
    } else {
        FILE* file = fopen(arguments.source_file_name, "r");
        if (!file) {
            fprintf(stderr, "Could not open file %s: %s.\n", arguments.source_file_name, strerror(errno));
            return EXIT_FAILURE;
        }
        size_t length;
        read_whole_file(file, &source_data, &length);
        fclose(file);
        source = (StringView){ .data = source_data, .length = length };
    }
    SourceFile source_file = {
        .filename = string_view_from_string(arguments.source_file_name == NULL ? "<stdin>" : arguments.source_file_name),
        .source = source,
    };

    ConstantsMap constants = constants_map_create(16);
    fill_constants_map(&constants);

    TokenVector tokens = tokenize(source_file, &constants);

    OpcodeList opcodes = opcode_specifications();
    check_opcodes(opcodes);

    InstructionMapVector instruction_map_vector;
    InstructionMapVector* instruction_map_pointer = NULL;
    if (arguments.instruction_map_file_name != NULL) {
        instruction_map_vector = instruction_map_vector_create();
        instruction_map_pointer = &instruction_map_vector;
    }

    ByteVector machine_code = parse(source_file, tokens, opcodes, &constants, instruction_map_pointer);

    // when in windows, we have to set the mode of stdout to binary because otherwise
    // every \n will be automatically replaced with \r\n which destroys the generated
    // binary
#ifdef _WIN32
    _setmode(_fileno(stdout), _O_BINARY);
#endif

    write_machine_code(machine_code, stdout);
    if (arguments.instruction_map_file_name != NULL) {
        write_instruction_map(instruction_map_vector, arguments.instruction_map_file_name);
    }

    // cleanup
    if (instruction_map_pointer != NULL) {
        instruction_map_vector_free(instruction_map_pointer);
    }
    byte_vector_free(&machine_code);
    token_vector_free(&tokens);
    constants_map_free(&constants);
    free(source_data);
    free(opcodes.specifications);
    return EXIT_SUCCESS;
}
