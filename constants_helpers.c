#include "upholsterer2k/constants.h"
#include <string.h>
#include <assert.h>

CREATE_HASHMAP_DEFINITION(ConstantsMap, Constant const*, constants_map)

void fill_constants_map(ConstantsMap* constants_map) {
    for (size_t i = 0; i < g_num_constant_abbreviations; ++i) {
        StringView abbreviation = string_view_from_string(g_constant_abbreviations[i].abbreviation);
        StringView abbreviation_mapping = string_view_from_string(g_constant_abbreviations[i].name);
        bool found = false;
        for (size_t j = 0; j < g_num_constants; ++j) {
            StringView name = string_view_from_string(g_constants[j].name);
            if (string_view_compare(abbreviation_mapping, name) == 0) {
                found = true;
                bool const did_not_exist_before = constants_map_insert(
                    constants_map,
                    abbreviation,
                    &g_constants[j]
                );
                assert(did_not_exist_before && "constant abbreviations must be unique");
                break;
            }
        }
        assert(found && "every constant abbreviation needs a corresponding constant");
    }
}

char const* find_constant_name_by_abbreviation(StringView abbreviation) {
    for (size_t i = 0; i < g_num_constant_abbreviations; ++i) {
        StringView current_abbreviation_view = string_view_from_string(g_constant_abbreviations[i].abbreviation);
        if (string_view_compare_case_insensitive(abbreviation, current_abbreviation_view) == 0) {
            return g_constant_abbreviations[i].name;
        }
    }
    return NULL;
}

void get_constant(StringView abbreviation, bool* out_found, Constant** out_constant) {
    char const* constant_name = find_constant_name_by_abbreviation(abbreviation);
    if (constant_name == NULL) {
        *out_found = false;
        return;
    }
    Constant* constant = NULL;
    for (size_t i = 0; i < g_num_constants; ++i) {
        if (strcmp(constant_name, g_constants[i].name) == 0) {
            *out_found = true;
            *out_constant = &g_constants[i];
            return;
        }
    }
    assert(constant != NULL && "constant with given name does not exist");
}

void get_constant_value(
    StringView abbreviation,
    ConstantType type,
    bool* out_found,
    uint64_t* out_value,
    ConstantsMap const* constants
) {
    Constant const** constant = constants_map_get(constants, abbreviation);
    if (constant == NULL || (*constant)->type != type) {
        *out_found = false;
        return;
    }
    *out_found = true;
    if (out_value != NULL) {
        *out_value = (*constant)->value;
    }
}
