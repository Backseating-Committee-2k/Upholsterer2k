#include "upholsterer2k/opcode_specification.h"
#include <string.h>
#include <assert.h>

StringView opcode_to_mnemonic(StringView const opcode) {
    #define X(code, mnemonic) if (string_view_compare(opcode, string_view_from_string(#code)) == 0) return string_view_from_string(#mnemonic) ;
    OPCODE_MAPPINGS
    #undef X
    return (StringView){
        .data = NULL,
        .length = 0,
    };
}
