#pragma once

#include <stdlib.h>
#include "opcode_specification.h"

typedef struct {
    OpcodeSpecification* specifications;
    size_t num_specifications;
} OpcodeList;

// the opcodes.c file must be auto-generated by ./tools/opcodes2c.py
OpcodeList opcode_specifications();
