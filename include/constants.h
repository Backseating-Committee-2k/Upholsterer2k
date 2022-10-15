#pragma once

#include "types.h"
#include "string_view.h"
#include "hashmap.h"
#include <stddef.h>
#include <stdbool.h>

typedef enum {
    CONSTANT_TYPE_ADDRESS,
    CONSTANT_TYPE_REGISTER,
    CONSTANT_TYPE_UNSIGNED_INTEGER,
} ConstantType;

typedef struct {
    char const* name;
    ConstantType type;
    uint64_t value;
} Constant;

typedef struct {
    char const* name;
    char const* abbreviation;
} ConstantAbbreviation;

CREATE_HASHMAP_DECLARATION(ConstantsMap, Constant const*, constants_map)

extern size_t g_num_constants;
extern Constant g_constants[];

extern size_t g_num_constant_abbreviations;
extern ConstantAbbreviation g_constant_abbreviations[];

char const* find_constant_name_by_abbreviation(StringView abbreviation);
void get_constant(StringView abbreviation, bool* out_found, Constant** out_constant);
void get_constant_value(StringView abbreviation,
                        ConstantType type,
                        bool* out_found,
                        uint64_t* out_value,
                        ConstantsMap const* constants);
void fill_constants_map(ConstantsMap* constants_map);
