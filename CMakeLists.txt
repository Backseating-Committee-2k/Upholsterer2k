# project (global) CMakeLists.txt file

cmake_minimum_required (VERSION 3.8)

project("Upholsterer2k" VERSION 0.1.0 DESCRIPTION "the official assembler for the backseat safe system 2k")

# set C language standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if (MSVC)
    # enable code analysis
    set_property(GLOBAL PROPERTY VS_GLOBAL_EnableCppCoreCheck true)
    set_property(GLOBAL PROPERTY VS_GLOBAL_CodeAnalysisRuleSet CppCoreCheckRules.ruleset)
    set_property(GLOBAL PROPERTY VS_GLOBAL_RunCodeAnalysis true)

    # remove /W3 from defaults
    string(REGEX REPLACE "/W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    string(REGEX REPLACE "-W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
endif ()

set(PUBLIC_HEADER_FILES
    "include/string_view.h"
    "include/string_view_utils.h"
    "include/token.h"
    "include/types.h"
    "include/token_vector.h"
    "include/lexer.h"
    "include/source_file.h"
    "include/error.h"
    "include/opcode_specification.h"
    "include/opcodes.h"
    "include/vector.h"
    "include/parser.h"
    "include/constants.h"
    "include/hashmap.h"
)

set(FILES
    "main.c"
    "string_view_utils.c"
    "string_view.c"
    "token.c"
    "token_vector.c"
    "lexer.c"
    "error.c"
    "opcode_specification.c"
    "opcodes.c"
    "parser.c"
    "constants.c"
    "constant_abbreviations.c"
    "constants_helpers.c"
    "hashmap.c"
    ${PUBLIC_HEADER_FILES}
)

set(TARGET_LIST "Upholsterer2k" "Upholsterer2kLib")

add_library(
    Upholsterer2kLib STATIC
    ${FILES}
)

add_executable(
    Upholsterer2k
    ${FILES}
)

set_target_properties(Upholsterer2kLib PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADER_FILES}")
target_include_directories(Upholsterer2kLib PUBLIC ./include)
target_link_libraries(Upholsterer2k PUBLIC Upholsterer2kLib)
target_include_directories(Upholsterer2k PUBLIC ./include)

include(GNUInstallDirs)

install(TARGETS Upholsterer2kLib
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

foreach (target ${TARGET_LIST})
    # set warning levels
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${target} PUBLIC /W4 /permissive-)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${target} PUBLIC -Wall -Wextra -Wconversion -Wswitch -Wimplicit-fallthrough -pedantic -fsanitize=address,undefined -Werror)
        target_link_options(${target} PUBLIC -fsanitize=address,undefined)
    endif ()

    # define DEBUG_BUILD
    target_compile_definitions(${target} PUBLIC "$<$<CONFIG:DEBUG>:DEBUG_BUILD>")
endforeach ()
