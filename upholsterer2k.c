#include <upholsterer2k/upholsterer2k.h>
#include <upholsterer2k/constants.h>
#include <upholsterer2k/lexer.h>

static void check_opcodes(OpcodeList const opcodes) {
    for (size_t i = 0; i < opcodes.num_specifications; ++i) {
        OpcodeSpecification const* const specification = &opcodes.specifications[i];
        assert(specification->mnemonic.length > 0 && "unknown opcode");
    }
}

bool bssemble(SourceFile const source_file,
              char const* const instruction_map_file_name,
              ByteVector* out_machine_code,
              InstructionMapVector* const out_instruction_map,
              char* const out_error_message_buffer,
              size_t const buffer_size) {
    (void)out_error_message_buffer;
    (void)buffer_size;

    ConstantsMap constants = constants_map_create(16);
    fill_constants_map(&constants);

    TokenVector tokens = tokenize(source_file, &constants);

    OpcodeList opcodes = opcode_specifications();
    check_opcodes(opcodes);

    if (instruction_map_file_name != NULL && out_instruction_map != NULL) {
        *out_instruction_map = instruction_map_vector_create();
    }

    ByteVector machine_code = parse(source_file, tokens, opcodes, &constants, out_instruction_map);
    *out_machine_code = machine_code;

    // cleanup
    token_vector_free(&tokens);
    constants_map_free(&constants);
    free(opcodes.specifications);

    return true;
}
