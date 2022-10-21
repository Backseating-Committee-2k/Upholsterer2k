#include "upholsterer2k/opcode_specification.h"
#include <string.h>
#include <assert.h>

UP2K_StringView opcode_to_mnemonic(UP2K_StringView const UP2K_Opcode) {
    #define X(code, mnemonic) if (UP2K_string_view_compare(UP2K_Opcode, UP2K_string_view_from_string(#code)) == 0) return UP2K_string_view_from_string(#mnemonic) ;
    OPCODE_MAPPINGS
    #undef X
    return (UP2K_StringView){
        .data = NULL,
        .length = 0,
    };
}
