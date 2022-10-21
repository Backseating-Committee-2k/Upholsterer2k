#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "upholsterer2k/types.h"
#include "upholsterer2k/string_view.h"
#include "upholsterer2k/hashmap.h"
#include <stddef.h>
#include <stdbool.h>

typedef enum {
    CONSTANT_TYPE_ADDRESS,
    CONSTANT_TYPE_REGISTER,
    CONSTANT_TYPE_UNSIGNED_INTEGER,
} UP2K_ConstantType;

typedef struct {
    char const* name;
    UP2K_ConstantType type;
    uint64_t value;
} UP2K_Constant;

typedef struct {
    char const* name;
    char const* abbreviation;
} UP2K_ConstantAbbreviation;

CREATE_HASHMAP_DECLARATION(UP2K_ConstantsMap, UP2K_Constant const*, constants_map)

extern size_t const UP2K_g_num_constants;
extern UP2K_Constant const UP2K_g_constants[];

extern size_t UP2K_g_num_constant_abbreviations;
extern UP2K_ConstantAbbreviation UP2K_g_constant_abbreviations[];

char const* UP2K_find_constant_name_by_abbreviation(UP2K_StringView abbreviation);
void UP2K_get_constant(UP2K_StringView abbreviation, bool* out_found, UP2K_Constant const** out_constant);
void UP2K_get_constant_value(UP2K_StringView abbreviation,
                        UP2K_ConstantType type,
                        bool* out_found,
                        uint64_t* out_value,
                        UP2K_ConstantsMap const* constants);
void UP2K_fill_constants_map(UP2K_ConstantsMap* constants_map);

#ifdef __cplusplus
}
#endif
