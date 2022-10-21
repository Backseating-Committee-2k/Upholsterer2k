#include <upholsterer2k/upholsterer2k.h>
#include <upholsterer2k/constants.h>
#include <upholsterer2k/lexer.h>

void UP2K_write_machine_code(UP2K_ByteVector const machine_code, FILE* const file) {
    fwrite(machine_code.data, sizeof(uint8_t), machine_code.size, file);
}

static void check_opcodes(UP2K_OpcodeList const opcodes) {
    for (size_t i = 0; i < opcodes.num_specifications; ++i) {
        UP2K_OpcodeSpecification const* const specification = &opcodes.specifications[i];
        assert(specification->mnemonic.length > 0 && "unknown UP2K_Opcode");
    }
}

bool UP2K_bssemble(UP2K_SourceFile const source_file,
              char const* const instruction_map_file_name,
              UP2K_ByteVector* out_machine_code,
              UP2K_InstructionMapVector* const out_instruction_map,
              char* const out_error_message_buffer,
              size_t const buffer_size) {
    (void)out_error_message_buffer;
    (void)buffer_size;

    UP2K_ConstantsMap constants = constants_map_create(16);
    UP2K_fill_constants_map(&constants);

    UP2K_TokenVector tokens = UP2K_tokenize(source_file, &constants);

    UP2K_OpcodeList opcodes = UP2K_opcode_specifications();
    check_opcodes(opcodes);

    if (instruction_map_file_name != NULL && out_instruction_map != NULL) {
        *out_instruction_map = UP2K_instruction_map_vector_create();
    }

    UP2K_ByteVector machine_code = UP2K_parse(source_file, tokens, opcodes, &constants, out_instruction_map);
    *out_machine_code = machine_code;

    // cleanup
    UP2K_token_vector_free(&tokens);
    constants_map_free(&constants);
    free(opcodes.specifications);

    return true;
}
