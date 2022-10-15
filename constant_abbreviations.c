#include "upholsterer2k/constants.h"
#include <stddef.h>

ConstantAbbreviation g_constant_abbreviations[] = {
    {
       .name = "DISPLAY_HEIGHT",
       .abbreviation = "DISPLAY_HEIGHT",
    },
    {
       .name = "DISPLAY_WIDTH",
       .abbreviation = "DISPLAY_WIDTH",
    },
    {
       .name = "ENTRY_POINT",
       .abbreviation = "ENTRY_POINT",
    },
    {
       .name = "FIRST_FRAMEBUFFER_START",
       .abbreviation = "FIRST_FRAMEBUFFER_START",
    },
    {
       .name = "FLAGS",
       .abbreviation = "FLAGS",
    },
    {
       .name = "FRAMEBUFFER_SIZE",
       .abbreviation = "FRAMEBUFFER_SIZE",
    },
    {
       .name = "INSTRUCTION_POINTER",
       .abbreviation = "IP",
    },
    {
       .name = "NUM_REGISTERS",
       .abbreviation = "NUM_REGISTERS",
    },
    {
       .name = "SECOND_FRAMEBUFFER_START",
       .abbreviation = "SECOND_FRAMEBUFFER_START",
    },
    {
       .name = "STACK_POINTER",
       .abbreviation = "SP",
    },
    {
       .name = "STACK_SIZE",
       .abbreviation = "STACK_SIZE",
    },
    {
       .name = "STACK_START",
       .abbreviation = "STACK_START",
    },
    {
       .name = "TERMINAL_BUFFER_SIZE",
       .abbreviation = "TERMINAL_SIZE",
    },
    {
       .name = "TERMINAL_BUFFER_START",
       .abbreviation = "TERMINAL_START",
    },
    {
       .name = "TERMINAL_BUFFER_END",
       .abbreviation = "TERMINAL_END",
    },
    {
       .name = "TERMINAL_CURSOR_POINTER",
       .abbreviation = "CURSOR_POINTER",
    },
    {
       .name = "TERMINAL_CURSOR_MODE",
       .abbreviation = "CURSOR_MODE",
    },
    {
       .name = "TERMINAL_CURSOR_MODE_BLINKING",
       .abbreviation = "CURSOR_MODE_BLINKING",
    },
    {
       .name = "TERMINAL_CURSOR_MODE_INVISIBLE",
       .abbreviation = "CURSOR_MODE_INVISIBLE",
    },
    {
       .name = "TERMINAL_CURSOR_MODE_VISIBLE",
       .abbreviation = "CURSOR_MODE_VISIBLE",
    },
    {
       .name = "TERMINAL_HEIGHT",
       .abbreviation = "TERMINAL_HEIGHT",
    },
    {
       .name = "TERMINAL_WIDTH",
       .abbreviation = "TERMINAL_WIDTH",
    },
};

size_t g_num_constant_abbreviations = sizeof(g_constant_abbreviations) / sizeof(g_constant_abbreviations[0]);
