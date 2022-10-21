#include "upholsterer2k/constants.h"
#include <string.h>
#include <assert.h>

CREATE_HASHMAP_DEFINITION(UP2K_ConstantsMap, UP2K_Constant const*, constants_map)

void UP2K_fill_constants_map(UP2K_ConstantsMap* constants_map) {
    for (size_t i = 0; i < UP2K_g_num_constant_abbreviations; ++i) {
        UP2K_StringView abbreviation = UP2K_string_view_from_string(UP2K_g_constant_abbreviations[i].abbreviation);
        UP2K_StringView abbreviation_mapping = UP2K_string_view_from_string(UP2K_g_constant_abbreviations[i].name);
        for (size_t j = 0; j < UP2K_g_num_constants; ++j) {
            UP2K_StringView name = UP2K_string_view_from_string(UP2K_g_constants[j].name);
            if (UP2K_string_view_compare(abbreviation_mapping, name) == 0) {
                bool const did_not_exist_before = constants_map_insert(
                    constants_map,
                    abbreviation,
                    &UP2K_g_constants[j]
                );
                (void)did_not_exist_before; // simulate [[maybe_unused]]
                assert(did_not_exist_before && "constant abbreviations must be unique");
                break;
            }
        }
        assert(found && "every constant abbreviation needs a corresponding constant");
    }
}

char const* UP2K_find_constant_name_by_abbreviation(UP2K_StringView abbreviation) {
    for (size_t i = 0; i < UP2K_g_num_constant_abbreviations; ++i) {
        UP2K_StringView current_abbreviation_view = UP2K_string_view_from_string(UP2K_g_constant_abbreviations[i].abbreviation);
        if (UP2K_string_view_compare_case_insensitive(abbreviation, current_abbreviation_view) == 0) {
            return UP2K_g_constant_abbreviations[i].name;
        }
    }
    return NULL;
}

void UP2K_get_constant(UP2K_StringView abbreviation, bool* out_found, UP2K_Constant const** out_constant) {
    char const* constant_name = UP2K_find_constant_name_by_abbreviation(abbreviation);
    if (constant_name == NULL) {
        *out_found = false;
        return;
    }
    UP2K_Constant* constant = NULL;
    for (size_t i = 0; i < UP2K_g_num_constants; ++i) {
        if (strcmp(constant_name, UP2K_g_constants[i].name) == 0) {
            *out_found = true;
            *out_constant = &UP2K_g_constants[i];
            return;
        }
    }
    (void)constant; // simulate [[maybe_unused]]
    assert(constant != NULL && "constant with given name does not exist");
}

void UP2K_get_constant_value(
    UP2K_StringView abbreviation,
    UP2K_ConstantType type,
    bool* out_found,
    uint64_t* out_value,
    UP2K_ConstantsMap const* constants
) {
    UP2K_Constant const** constant = constants_map_get(constants, abbreviation);
    if (constant == NULL || (*constant)->type != type) {
        *out_found = false;
        return;
    }
    *out_found = true;
    if (out_value != NULL) {
        *out_value = (*constant)->value;
    }
}
