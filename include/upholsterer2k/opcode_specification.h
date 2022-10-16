#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "upholsterer2k/string_view.h"

typedef enum {
    ARGUMENT_TYPE_NONE,
    ARGUMENT_TYPE_IMMEDIATE,
    ARGUMENT_TYPE_ADDRESS_POINTER,
    ARGUMENT_TYPE_REGISTER,
    ARGUMENT_TYPE_REGISTER_POINTER,
    // TODO: Difference?
} ArgumentType;

typedef uint16_t Opcode;

typedef struct {
    StringView name;                   // e.g. "MoveRegisterImmediate"
    StringView mnemonic;               // e.g. "COPY"
    size_t argument_count;             // e.g. 2
    ArgumentType required_arguments[6];// e.g. { Immediate, Register }
    uint8_t offsets[6];                // amount to shift the argument to the right (in bits)
    Opcode opcode;                     // e.g. 0x0000
} OpcodeSpecification;

#define MNEMONICS               \
    X(COPY)                     \
    X(COPY_BYTE)                \
    X(COPY_HALFWORD)            \
    X(OFFSET_COPY)              \
    X(OFFSET_COPY_BYTE)         \
    X(OFFSET_COPY_HALFWORD)     \
    X(HALT)                     \
    X(ADD)                      \
    X(ADD_CARRY)                \
    X(SUB)                      \
    X(SUB_CARRY)                \
    X(MULT)                     \
    X(DIVMOD)                   \
    X(AND)                      \
    X(OR)                       \
    X(XOR)                      \
    X(NOT)                      \
    X(LSHIFT)                   \
    X(RSHIFT)                   \
    X(COMP)                     \
    X(PUSH)                     \
    X(POP)                      \
    X(CALL)                     \
    X(RETURN)                   \
    X(JUMP)                     \
    X(JUMP_EQ)                  \
    X(JUMP_GT)                  \
    X(JUMP_LT)                  \
    X(JUMP_GTEQ)                \
    X(JUMP_LTEQ)                \
    X(JUMP_ZERO)                \
    X(JUMP_NOT_ZERO)            \
    X(JUMP_CARRY)               \
    X(JUMP_NOT_CARRY)           \
    X(JUMP_DIVIDE_BY_ZERO)      \
    X(JUMP_NOT_DIVIDE_BY_ZERO)  \
    X(NOOP)                     \
    X(GET_KEY)                  \
    X(POLL_TIME)                \
    X(SWAP)                     \
    X(DRAW_BUFFER_ADDR)         \
    X(POLL_CYCLES)              \
    X(DUMP_REGISTERS)           \
    X(DUMP_MEMORY)              \
    X(ASSERT)                   \
    X(DEBUG_BREAK)              \
    X(PRINT)                    \
    X(COMP_EQ)                  \
    X(COMP_NEQ)                 \
    X(COMP_GT)                  \
    X(COMP_GE)                  \
    X(COMP_LT)                  \
    X(COMP_LE)                  \
    X(CHECKPOINT)

#define OPCODE_MAPPINGS                                        \
    X(MoveRegisterImmediate, COPY)                             \
    X(MoveRegisterAddress, COPY)                               \
    X(MoveTargetSource, COPY)                                  \
    X(MoveAddressRegister, COPY)                               \
    X(MoveTargetPointer, COPY)                                 \
    X(MovePointerSource, COPY)                                 \
    X(MoveByteRegisterAddress, COPY_BYTE)                      \
    X(MoveByteAddressRegister, COPY_BYTE)                      \
    X(MoveByteTargetPointer, COPY_BYTE)                        \
    X(MoveBytePointerSource, COPY_BYTE)                        \
    X(MoveHalfwordRegisterAddress, COPY_HALFWORD)              \
    X(MoveHalfwordAddressRegister, COPY_HALFWORD)              \
    X(MoveHalfwordTargetPointer, COPY_HALFWORD)                \
    X(MoveHalfwordPointerSource, COPY_HALFWORD)                \
    X(MovePointerSourceOffset, OFFSET_COPY)                    \
    X(MoveBytePointerSourceOffset, OFFSET_COPY_BYTE)           \
    X(MoveHalfwordPointerSourceOffset, OFFSET_COPY_HALFWORD)   \
    X(MoveTargetPointerOffset, OFFSET_COPY)                    \
    X(MoveByteTargetPointerOffset, OFFSET_COPY_BYTE)           \
    X(MoveHalfwordTargetPointerOffset, OFFSET_COPY_HALFWORD)   \
    X(HaltAndCatchFire, HALT)                                  \
    X(AddTargetLhsRhs, ADD)                                    \
    X(AddWithCarryTargetLhsRhs, ADD_CARRY)                     \
    X(SubtractTargetLhsRhs, SUB)                               \
    X(SubtractWithCarryTargetLhsRhs, SUB_CARRY)                \
    X(MultiplyHighLowLhsRhs, MULT)                             \
    X(DivmodTargetModLhsRhs, DIVMOD)                           \
    X(AndTargetLhsRhs, AND)                                    \
    X(OrTargetLhsRhs, OR)                                      \
    X(XorTargetLhsRhs, XOR)                                    \
    X(NotTargetSource, NOT)                                    \
    X(LeftShiftTargetLhsRhs, LSHIFT)                           \
    X(RightShiftTargetLhsRhs, RSHIFT)                          \
    X(AddTargetSourceImmediate, ADD)                           \
    X(SubtractTargetSourceImmediate, SUB)                      \
    X(CompareTargetLhsRhs, COMP)                               \
    X(PushRegister, PUSH)                                      \
    X(PushImmediate, PUSH)                                     \
    X(PopRegister, POP)                                        \
    X(Pop, POP)                                                \
    X(CallImmediate, CALL)                                     \
    X(CallRegister, CALL)                                      \
    X(CallPointer, CALL)                                       \
    X(Return, RETURN)                                          \
    X(JumpImmediate, JUMP)                                     \
    X(JumpRegister, JUMP)                                      \
                                                               \
    X(JumpImmediateIfEqual, JUMP_EQ)                           \
    X(JumpImmediateIfGreaterThan, JUMP_GT)                     \
    X(JumpImmediateIfLessThan, JUMP_LT)                        \
    X(JumpImmediateIfGreaterThanOrEqual, JUMP_GTEQ)            \
    X(JumpImmediateIfLessThanOrEqual, JUMP_LTEQ)               \
    X(JumpImmediateIfZero, JUMP_ZERO)                          \
    X(JumpImmediateIfNotZero, JUMP_NOT_ZERO)                   \
    X(JumpImmediateIfCarry, JUMP_CARRY)                        \
    X(JumpImmediateIfNotCarry, JUMP_NOT_CARRY)                 \
    X(JumpImmediateIfDivideByZero, JUMP_DIVIDE_BY_ZERO)        \
    X(JumpImmediateIfNotDivideByZero, JUMP_NOT_DIVIDE_BY_ZERO) \
                                                               \
    X(JumpRegisterIfEqual, JUMP_EQ)                            \
    X(JumpRegisterIfGreaterThan, JUMP_GT)                      \
    X(JumpRegisterIfLessThan, JUMP_LT)                         \
    X(JumpRegisterIfGreaterThanOrEqual, JUMP_GTEQ)             \
    X(JumpRegisterIfLessThanOrEqual, JUMP_LTEQ)                \
    X(JumpRegisterIfZero, JUMP_ZERO)                           \
    X(JumpRegisterIfNotZero, JUMP_NOT_ZERO)                    \
    X(JumpRegisterIfCarry, JUMP_CARRY)                         \
    X(JumpRegisterIfNotCarry, JUMP_NOT_CARRY)                  \
    X(JumpRegisterIfDivideByZero, JUMP_DIVIDE_BY_ZERO)         \
    X(JumpRegisterIfNotDivideByZero, JUMP_NOT_DIVIDE_BY_ZERO)  \
                                                               \
    X(NoOp, NOOP)                                              \
    X(GetKeyState, GET_KEY)                                    \
    X(PollTime, POLL_TIME)                                     \
    X(SwapFramebuffers, SWAP)                                  \
    X(InvisibleFramebufferAddress, DRAW_BUFFER_ADDR)           \
    X(PollCycleCountHighLow, POLL_CYCLES)                      \
                                                               \
    X(DumpRegisters, DUMP_REGISTERS)                           \
    X(DumpMemory, DUMP_MEMORY)                                 \
    X(AssertRegisterRegister, ASSERT)                          \
    X(AssertRegisterImmediate, ASSERT)                         \
    X(AssertPointerImmediate, ASSERT)                          \
    X(DebugBreak, DEBUG_BREAK)                                 \
    X(PrintRegister, PRINT)                                    \
    X(BoolCompareEquals, COMP_EQ)                              \
    X(BoolCompareNotEquals, COMP_NEQ)                          \
    X(BoolCompareGreater, COMP_GT)                             \
    X(BoolCompareGreaterOrEquals, COMP_GE)                     \
    X(BoolCompareLess, COMP_LT)                                \
    X(BoolCompareLessOrEquals, COMP_LE)                        \
    X(Checkpoint, CHECKPOINT)

StringView opcode_to_mnemonic(StringView opcode);

#ifdef __cplusplus
}
#endif
