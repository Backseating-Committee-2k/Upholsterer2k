// auto-generated with opcodes2c.py

#include "upholsterer2k/opcodes.h"
#include "upholsterer2k/string_view.h"

OpcodeList opcode_specifications(void) {
    size_t const num_opcodes = 88;
    OpcodeSpecification* specifications = malloc(num_opcodes * sizeof(*specifications));
    specifications[0] = (OpcodeSpecification){
        .name = string_view_from_string("MoveRegisterImmediate"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("MoveRegisterImmediate")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_IMMEDIATE, ARGUMENT_TYPE_REGISTER },
        .offsets = { 0, 40 },
        .opcode = 0x0,
    };
    specifications[1] = (OpcodeSpecification){
        .name = string_view_from_string("MoveRegisterAddress"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("MoveRegisterAddress")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_ADDRESS_POINTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 0, 40 },
        .opcode = 0x1,
    };
    specifications[2] = (OpcodeSpecification){
        .name = string_view_from_string("MoveTargetSource"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("MoveTargetSource")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 40 },
        .opcode = 0x2,
    };
    specifications[3] = (OpcodeSpecification){
        .name = string_view_from_string("MoveAddressRegister"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("MoveAddressRegister")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_ADDRESS_POINTER },
        .offsets = { 40, 0 },
        .opcode = 0x3,
    };
    specifications[4] = (OpcodeSpecification){
        .name = string_view_from_string("MoveTargetPointer"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("MoveTargetPointer")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER_POINTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 40 },
        .opcode = 0x4,
    };
    specifications[5] = (OpcodeSpecification){
        .name = string_view_from_string("MovePointerSource"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("MovePointerSource")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER_POINTER },
        .offsets = { 32, 40 },
        .opcode = 0x5,
    };
    specifications[6] = (OpcodeSpecification){
        .name = string_view_from_string("HaltAndCatchFire"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("HaltAndCatchFire")),
        .argument_count = 0,
        .required_arguments = { ARGUMENT_TYPE_NONE },
        .offsets = { 0 },
        .opcode = 0x6,
    };
    specifications[7] = (OpcodeSpecification){
        .name = string_view_from_string("AddTargetLhsRhs"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("AddTargetLhsRhs")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 24, 40 },
        .opcode = 0x7,
    };
    specifications[8] = (OpcodeSpecification){
        .name = string_view_from_string("SubtractTargetLhsRhs"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("SubtractTargetLhsRhs")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 24, 40 },
        .opcode = 0x8,
    };
    specifications[9] = (OpcodeSpecification){
        .name = string_view_from_string("SubtractWithCarryTargetLhsRhs"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("SubtractWithCarryTargetLhsRhs")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 24, 40 },
        .opcode = 0x9,
    };
    specifications[10] = (OpcodeSpecification){
        .name = string_view_from_string("MultiplyHighLowLhsRhs"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("MultiplyHighLowLhsRhs")),
        .argument_count = 4,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 24, 16, 40, 32 },
        .opcode = 0xa,
    };
    specifications[11] = (OpcodeSpecification){
        .name = string_view_from_string("DivmodTargetModLhsRhs"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("DivmodTargetModLhsRhs")),
        .argument_count = 4,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 24, 16, 40, 32 },
        .opcode = 0xb,
    };
    specifications[12] = (OpcodeSpecification){
        .name = string_view_from_string("AndTargetLhsRhs"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("AndTargetLhsRhs")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 24, 40 },
        .opcode = 0xc,
    };
    specifications[13] = (OpcodeSpecification){
        .name = string_view_from_string("OrTargetLhsRhs"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("OrTargetLhsRhs")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 24, 40 },
        .opcode = 0xd,
    };
    specifications[14] = (OpcodeSpecification){
        .name = string_view_from_string("XorTargetLhsRhs"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("XorTargetLhsRhs")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 24, 40 },
        .opcode = 0xe,
    };
    specifications[15] = (OpcodeSpecification){
        .name = string_view_from_string("NotTargetSource"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("NotTargetSource")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 40 },
        .opcode = 0xf,
    };
    specifications[16] = (OpcodeSpecification){
        .name = string_view_from_string("LeftShiftTargetLhsRhs"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("LeftShiftTargetLhsRhs")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 24, 40 },
        .opcode = 0x10,
    };
    specifications[17] = (OpcodeSpecification){
        .name = string_view_from_string("RightShiftTargetLhsRhs"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("RightShiftTargetLhsRhs")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 24, 40 },
        .opcode = 0x11,
    };
    specifications[18] = (OpcodeSpecification){
        .name = string_view_from_string("AddTargetSourceImmediate"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("AddTargetSourceImmediate")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_IMMEDIATE, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 0, 40 },
        .opcode = 0x12,
    };
    specifications[19] = (OpcodeSpecification){
        .name = string_view_from_string("SubtractTargetSourceImmediate"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("SubtractTargetSourceImmediate")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_IMMEDIATE, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 0, 40 },
        .opcode = 0x13,
    };
    specifications[20] = (OpcodeSpecification){
        .name = string_view_from_string("CompareTargetLhsRhs"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("CompareTargetLhsRhs")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 24, 40 },
        .opcode = 0x14,
    };
    specifications[21] = (OpcodeSpecification){
        .name = string_view_from_string("PushRegister"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("PushRegister")),
        .argument_count = 1,
        .required_arguments = { ARGUMENT_TYPE_REGISTER },
        .offsets = { 40 },
        .opcode = 0x15,
    };
    specifications[22] = (OpcodeSpecification){
        .name = string_view_from_string("PopRegister"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("PopRegister")),
        .argument_count = 1,
        .required_arguments = { ARGUMENT_TYPE_REGISTER },
        .offsets = { 40 },
        .opcode = 0x16,
    };
    specifications[23] = (OpcodeSpecification){
        .name = string_view_from_string("CallImmediate"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("CallImmediate")),
        .argument_count = 1,
        .required_arguments = { ARGUMENT_TYPE_IMMEDIATE },
        .offsets = { 0 },
        .opcode = 0x17,
    };
    specifications[24] = (OpcodeSpecification){
        .name = string_view_from_string("Return"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("Return")),
        .argument_count = 0,
        .required_arguments = { ARGUMENT_TYPE_NONE },
        .offsets = { 0 },
        .opcode = 0x18,
    };
    specifications[25] = (OpcodeSpecification){
        .name = string_view_from_string("JumpImmediate"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpImmediate")),
        .argument_count = 1,
        .required_arguments = { ARGUMENT_TYPE_IMMEDIATE },
        .offsets = { 0 },
        .opcode = 0x19,
    };
    specifications[26] = (OpcodeSpecification){
        .name = string_view_from_string("JumpRegister"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpRegister")),
        .argument_count = 1,
        .required_arguments = { ARGUMENT_TYPE_REGISTER },
        .offsets = { 40 },
        .opcode = 0x1a,
    };
    specifications[27] = (OpcodeSpecification){
        .name = string_view_from_string("JumpImmediateIfEqual"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpImmediateIfEqual")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_IMMEDIATE },
        .offsets = { 40, 0 },
        .opcode = 0x1b,
    };
    specifications[28] = (OpcodeSpecification){
        .name = string_view_from_string("JumpImmediateIfGreaterThan"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpImmediateIfGreaterThan")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_IMMEDIATE },
        .offsets = { 40, 0 },
        .opcode = 0x1c,
    };
    specifications[29] = (OpcodeSpecification){
        .name = string_view_from_string("JumpImmediateIfLessThan"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpImmediateIfLessThan")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_IMMEDIATE },
        .offsets = { 40, 0 },
        .opcode = 0x1d,
    };
    specifications[30] = (OpcodeSpecification){
        .name = string_view_from_string("JumpImmediateIfGreaterThanOrEqual"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpImmediateIfGreaterThanOrEqual")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_IMMEDIATE },
        .offsets = { 40, 0 },
        .opcode = 0x1e,
    };
    specifications[31] = (OpcodeSpecification){
        .name = string_view_from_string("JumpImmediateIfLessThanOrEqual"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpImmediateIfLessThanOrEqual")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_IMMEDIATE },
        .offsets = { 40, 0 },
        .opcode = 0x1f,
    };
    specifications[32] = (OpcodeSpecification){
        .name = string_view_from_string("JumpImmediateIfZero"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpImmediateIfZero")),
        .argument_count = 1,
        .required_arguments = { ARGUMENT_TYPE_IMMEDIATE },
        .offsets = { 0 },
        .opcode = 0x20,
    };
    specifications[33] = (OpcodeSpecification){
        .name = string_view_from_string("JumpImmediateIfNotZero"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpImmediateIfNotZero")),
        .argument_count = 1,
        .required_arguments = { ARGUMENT_TYPE_IMMEDIATE },
        .offsets = { 0 },
        .opcode = 0x21,
    };
    specifications[34] = (OpcodeSpecification){
        .name = string_view_from_string("JumpImmediateIfCarry"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpImmediateIfCarry")),
        .argument_count = 1,
        .required_arguments = { ARGUMENT_TYPE_IMMEDIATE },
        .offsets = { 0 },
        .opcode = 0x22,
    };
    specifications[35] = (OpcodeSpecification){
        .name = string_view_from_string("JumpImmediateIfNotCarry"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpImmediateIfNotCarry")),
        .argument_count = 1,
        .required_arguments = { ARGUMENT_TYPE_IMMEDIATE },
        .offsets = { 0 },
        .opcode = 0x23,
    };
    specifications[36] = (OpcodeSpecification){
        .name = string_view_from_string("JumpImmediateIfDivideByZero"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpImmediateIfDivideByZero")),
        .argument_count = 1,
        .required_arguments = { ARGUMENT_TYPE_IMMEDIATE },
        .offsets = { 0 },
        .opcode = 0x24,
    };
    specifications[37] = (OpcodeSpecification){
        .name = string_view_from_string("JumpImmediateIfNotDivideByZero"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpImmediateIfNotDivideByZero")),
        .argument_count = 1,
        .required_arguments = { ARGUMENT_TYPE_IMMEDIATE },
        .offsets = { 0 },
        .opcode = 0x25,
    };
    specifications[38] = (OpcodeSpecification){
        .name = string_view_from_string("JumpRegisterIfEqual"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpRegisterIfEqual")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER_POINTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 40, 32 },
        .opcode = 0x26,
    };
    specifications[39] = (OpcodeSpecification){
        .name = string_view_from_string("JumpRegisterIfGreaterThan"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpRegisterIfGreaterThan")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER_POINTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 40, 32 },
        .opcode = 0x27,
    };
    specifications[40] = (OpcodeSpecification){
        .name = string_view_from_string("JumpRegisterIfLessThan"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpRegisterIfLessThan")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER_POINTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 40, 32 },
        .opcode = 0x28,
    };
    specifications[41] = (OpcodeSpecification){
        .name = string_view_from_string("JumpRegisterIfGreaterThanOrEqual"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpRegisterIfGreaterThanOrEqual")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER_POINTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 40, 32 },
        .opcode = 0x29,
    };
    specifications[42] = (OpcodeSpecification){
        .name = string_view_from_string("JumpRegisterIfLessThanOrEqual"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpRegisterIfLessThanOrEqual")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER_POINTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 40, 32 },
        .opcode = 0x2a,
    };
    specifications[43] = (OpcodeSpecification){
        .name = string_view_from_string("JumpRegisterIfZero"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpRegisterIfZero")),
        .argument_count = 1,
        .required_arguments = { ARGUMENT_TYPE_REGISTER_POINTER },
        .offsets = { 40 },
        .opcode = 0x2b,
    };
    specifications[44] = (OpcodeSpecification){
        .name = string_view_from_string("JumpRegisterIfNotZero"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpRegisterIfNotZero")),
        .argument_count = 1,
        .required_arguments = { ARGUMENT_TYPE_REGISTER_POINTER },
        .offsets = { 40 },
        .opcode = 0x2c,
    };
    specifications[45] = (OpcodeSpecification){
        .name = string_view_from_string("JumpRegisterIfCarry"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpRegisterIfCarry")),
        .argument_count = 1,
        .required_arguments = { ARGUMENT_TYPE_REGISTER_POINTER },
        .offsets = { 40 },
        .opcode = 0x2d,
    };
    specifications[46] = (OpcodeSpecification){
        .name = string_view_from_string("JumpRegisterIfNotCarry"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpRegisterIfNotCarry")),
        .argument_count = 1,
        .required_arguments = { ARGUMENT_TYPE_REGISTER_POINTER },
        .offsets = { 40 },
        .opcode = 0x2e,
    };
    specifications[47] = (OpcodeSpecification){
        .name = string_view_from_string("JumpRegisterIfDivideByZero"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpRegisterIfDivideByZero")),
        .argument_count = 1,
        .required_arguments = { ARGUMENT_TYPE_REGISTER_POINTER },
        .offsets = { 40 },
        .opcode = 0x2f,
    };
    specifications[48] = (OpcodeSpecification){
        .name = string_view_from_string("JumpRegisterIfNotDivideByZero"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("JumpRegisterIfNotDivideByZero")),
        .argument_count = 1,
        .required_arguments = { ARGUMENT_TYPE_REGISTER_POINTER },
        .offsets = { 40 },
        .opcode = 0x30,
    };
    specifications[49] = (OpcodeSpecification){
        .name = string_view_from_string("NoOp"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("NoOp")),
        .argument_count = 0,
        .required_arguments = { ARGUMENT_TYPE_NONE },
        .offsets = { 0 },
        .opcode = 0x31,
    };
    specifications[50] = (OpcodeSpecification){
        .name = string_view_from_string("GetKeyState"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("GetKeyState")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 40 },
        .opcode = 0x32,
    };
    specifications[51] = (OpcodeSpecification){
        .name = string_view_from_string("PollTime"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("PollTime")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 40, 32 },
        .opcode = 0x33,
    };
    specifications[52] = (OpcodeSpecification){
        .name = string_view_from_string("AddWithCarryTargetLhsRhs"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("AddWithCarryTargetLhsRhs")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 24, 40 },
        .opcode = 0x34,
    };
    specifications[53] = (OpcodeSpecification){
        .name = string_view_from_string("SwapFramebuffers"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("SwapFramebuffers")),
        .argument_count = 0,
        .required_arguments = { ARGUMENT_TYPE_NONE },
        .offsets = { 0 },
        .opcode = 0x35,
    };
    specifications[54] = (OpcodeSpecification){
        .name = string_view_from_string("CallRegister"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("CallRegister")),
        .argument_count = 1,
        .required_arguments = { ARGUMENT_TYPE_REGISTER },
        .offsets = { 40 },
        .opcode = 0x36,
    };
    specifications[55] = (OpcodeSpecification){
        .name = string_view_from_string("CallPointer"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("CallPointer")),
        .argument_count = 1,
        .required_arguments = { ARGUMENT_TYPE_REGISTER_POINTER },
        .offsets = { 40 },
        .opcode = 0x37,
    };
    specifications[56] = (OpcodeSpecification){
        .name = string_view_from_string("InvisibleFramebufferAddress"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("InvisibleFramebufferAddress")),
        .argument_count = 1,
        .required_arguments = { ARGUMENT_TYPE_REGISTER },
        .offsets = { 40 },
        .opcode = 0x38,
    };
    specifications[57] = (OpcodeSpecification){
        .name = string_view_from_string("PollCycleCountHighLow"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("PollCycleCountHighLow")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 40, 32 },
        .opcode = 0x39,
    };
    specifications[58] = (OpcodeSpecification){
        .name = string_view_from_string("BoolCompareEquals"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("BoolCompareEquals")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 24, 40 },
        .opcode = 0x3a,
    };
    specifications[59] = (OpcodeSpecification){
        .name = string_view_from_string("BoolCompareNotEquals"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("BoolCompareNotEquals")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 24, 40 },
        .opcode = 0x3b,
    };
    specifications[60] = (OpcodeSpecification){
        .name = string_view_from_string("BoolCompareGreater"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("BoolCompareGreater")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 24, 40 },
        .opcode = 0x3c,
    };
    specifications[61] = (OpcodeSpecification){
        .name = string_view_from_string("BoolCompareGreaterOrEquals"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("BoolCompareGreaterOrEquals")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 24, 40 },
        .opcode = 0x3d,
    };
    specifications[62] = (OpcodeSpecification){
        .name = string_view_from_string("BoolCompareLess"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("BoolCompareLess")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 24, 40 },
        .opcode = 0x3e,
    };
    specifications[63] = (OpcodeSpecification){
        .name = string_view_from_string("BoolCompareLessOrEquals"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("BoolCompareLessOrEquals")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 24, 40 },
        .opcode = 0x3f,
    };
    specifications[64] = (OpcodeSpecification){
        .name = string_view_from_string("Pop"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("Pop")),
        .argument_count = 0,
        .required_arguments = { ARGUMENT_TYPE_NONE },
        .offsets = { 0 },
        .opcode = 0x40,
    };
    specifications[65] = (OpcodeSpecification){
        .name = string_view_from_string("MoveByteRegisterAddress"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("MoveByteRegisterAddress")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_ADDRESS_POINTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 0, 40 },
        .opcode = 0x41,
    };
    specifications[66] = (OpcodeSpecification){
        .name = string_view_from_string("MoveByteAddressRegister"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("MoveByteAddressRegister")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_ADDRESS_POINTER },
        .offsets = { 40, 0 },
        .opcode = 0x42,
    };
    specifications[67] = (OpcodeSpecification){
        .name = string_view_from_string("MoveByteTargetPointer"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("MoveByteTargetPointer")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER_POINTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 40 },
        .opcode = 0x43,
    };
    specifications[68] = (OpcodeSpecification){
        .name = string_view_from_string("MoveBytePointerSource"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("MoveBytePointerSource")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER_POINTER },
        .offsets = { 32, 40 },
        .opcode = 0x44,
    };
    specifications[69] = (OpcodeSpecification){
        .name = string_view_from_string("MoveHalfwordRegisterAddress"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("MoveHalfwordRegisterAddress")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_ADDRESS_POINTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 0, 40 },
        .opcode = 0x45,
    };
    specifications[70] = (OpcodeSpecification){
        .name = string_view_from_string("MoveHalfwordAddressRegister"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("MoveHalfwordAddressRegister")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_ADDRESS_POINTER },
        .offsets = { 40, 0 },
        .opcode = 0x46,
    };
    specifications[71] = (OpcodeSpecification){
        .name = string_view_from_string("MoveHalfwordTargetPointer"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("MoveHalfwordTargetPointer")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER_POINTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 40 },
        .opcode = 0x47,
    };
    specifications[72] = (OpcodeSpecification){
        .name = string_view_from_string("MoveHalfwordPointerSource"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("MoveHalfwordPointerSource")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER_POINTER },
        .offsets = { 32, 40 },
        .opcode = 0x48,
    };
    specifications[73] = (OpcodeSpecification){
        .name = string_view_from_string("MovePointerSourceOffset"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("MovePointerSourceOffset")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_IMMEDIATE, ARGUMENT_TYPE_REGISTER_POINTER },
        .offsets = { 32, 0, 40 },
        .opcode = 0x49,
    };
    specifications[74] = (OpcodeSpecification){
        .name = string_view_from_string("MoveBytePointerSourceOffset"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("MoveBytePointerSourceOffset")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_IMMEDIATE, ARGUMENT_TYPE_REGISTER_POINTER },
        .offsets = { 32, 0, 40 },
        .opcode = 0x4a,
    };
    specifications[75] = (OpcodeSpecification){
        .name = string_view_from_string("MoveHalfwordPointerSourceOffset"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("MoveHalfwordPointerSourceOffset")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_IMMEDIATE, ARGUMENT_TYPE_REGISTER_POINTER },
        .offsets = { 32, 0, 40 },
        .opcode = 0x4b,
    };
    specifications[76] = (OpcodeSpecification){
        .name = string_view_from_string("MoveTargetPointerOffset"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("MoveTargetPointerOffset")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER_POINTER, ARGUMENT_TYPE_IMMEDIATE, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 0, 40 },
        .opcode = 0x4c,
    };
    specifications[77] = (OpcodeSpecification){
        .name = string_view_from_string("MoveByteTargetPointerOffset"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("MoveByteTargetPointerOffset")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER_POINTER, ARGUMENT_TYPE_IMMEDIATE, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 0, 40 },
        .opcode = 0x4d,
    };
    specifications[78] = (OpcodeSpecification){
        .name = string_view_from_string("MoveHalfwordTargetPointerOffset"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("MoveHalfwordTargetPointerOffset")),
        .argument_count = 3,
        .required_arguments = { ARGUMENT_TYPE_REGISTER_POINTER, ARGUMENT_TYPE_IMMEDIATE, ARGUMENT_TYPE_REGISTER },
        .offsets = { 32, 0, 40 },
        .opcode = 0x4e,
    };
    specifications[79] = (OpcodeSpecification){
        .name = string_view_from_string("PushImmediate"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("PushImmediate")),
        .argument_count = 1,
        .required_arguments = { ARGUMENT_TYPE_IMMEDIATE },
        .offsets = { 0 },
        .opcode = 0x4f,
    };
    specifications[80] = (OpcodeSpecification){
        .name = string_view_from_string("Checkpoint"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("Checkpoint")),
        .argument_count = 1,
        .required_arguments = { ARGUMENT_TYPE_IMMEDIATE },
        .offsets = { 0 },
        .opcode = 0xfff8,
    };
    specifications[81] = (OpcodeSpecification){
        .name = string_view_from_string("PrintRegister"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("PrintRegister")),
        .argument_count = 1,
        .required_arguments = { ARGUMENT_TYPE_REGISTER },
        .offsets = { 40 },
        .opcode = 0xfff9,
    };
    specifications[82] = (OpcodeSpecification){
        .name = string_view_from_string("DebugBreak"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("DebugBreak")),
        .argument_count = 0,
        .required_arguments = { ARGUMENT_TYPE_NONE },
        .offsets = { 0 },
        .opcode = 0xfffa,
    };
    specifications[83] = (OpcodeSpecification){
        .name = string_view_from_string("AssertPointerImmediate"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("AssertPointerImmediate")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER_POINTER, ARGUMENT_TYPE_IMMEDIATE },
        .offsets = { 40, 0 },
        .opcode = 0xfffb,
    };
    specifications[84] = (OpcodeSpecification){
        .name = string_view_from_string("AssertRegisterImmediate"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("AssertRegisterImmediate")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_IMMEDIATE },
        .offsets = { 40, 0 },
        .opcode = 0xfffc,
    };
    specifications[85] = (OpcodeSpecification){
        .name = string_view_from_string("AssertRegisterRegister"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("AssertRegisterRegister")),
        .argument_count = 2,
        .required_arguments = { ARGUMENT_TYPE_REGISTER, ARGUMENT_TYPE_REGISTER },
        .offsets = { 40, 32 },
        .opcode = 0xfffd,
    };
    specifications[86] = (OpcodeSpecification){
        .name = string_view_from_string("DumpMemory"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("DumpMemory")),
        .argument_count = 0,
        .required_arguments = { ARGUMENT_TYPE_NONE },
        .offsets = { 0 },
        .opcode = 0xfffe,
    };
    specifications[87] = (OpcodeSpecification){
        .name = string_view_from_string("DumpRegisters"),
        .mnemonic = opcode_to_mnemonic(string_view_from_string("DumpRegisters")),
        .argument_count = 0,
        .required_arguments = { ARGUMENT_TYPE_NONE },
        .offsets = { 0 },
        .opcode = 0xffff,
    };
    return (OpcodeList){
    .specifications = specifications,
    .num_specifications = num_opcodes,
};
}
