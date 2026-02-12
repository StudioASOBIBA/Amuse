# --------------------
# Core module settings
# --------------------

# Source/layout roots for the core module.
set(AMUSE_CORE_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(AMUSE_CORE_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Gather header files for IDE visibility and alias generation.
file(
    GLOB_RECURSE
    AMUSE_CORE_HEADERS
    CONFIGURE_DEPENDS
    "${AMUSE_CORE_HEADERS_DIR}/*.h"
    "${AMUSE_CORE_HEADERS_DIR}/*.hpp"
)

# Fall back when this module is built standalone.
if (NOT DEFINED AMUSE_RUNTIME_GENERATED_INCLUDE_DIR)
    set(AMUSE_RUNTIME_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")
endif()

# Generated alias include root: <amuse/core/...>.
set(AMUSE_CORE_GENERATED_INCLUDE_DIR "${AMUSE_RUNTIME_GENERATED_INCLUDE_DIR}/amuse/core")

# Create alias headers that forward to the real paths.
foreach(header_file IN LISTS AMUSE_CORE_HEADERS)
    # Compute relative path from include/ to preserve folder layout.
    file(RELATIVE_PATH rel_header_file "${AMUSE_CORE_HEADERS_DIR}" "${header_file}")
    file(TO_CMAKE_PATH "${rel_header_file}" rel_header_file)

    # Resolve output location inside the generated include tree.
    set(alias_header_file "${AMUSE_CORE_GENERATED_INCLUDE_DIR}/${rel_header_file}")
    get_filename_component(alias_header_dir "${alias_header_file}" DIRECTORY)
    file(MAKE_DIRECTORY "${alias_header_dir}")

    # Alias contents: single include of the real header.
    set(alias_header_content "#pragma once\n#include <${rel_header_file}>\n")

    # Avoid rewriting if the alias is already up-to-date.
    if (EXISTS "${alias_header_file}")
        file(READ "${alias_header_file}" existing_alias_header_content)
        if (existing_alias_header_content STREQUAL alias_header_content)
            continue()
        endif()
    endif()

    file(WRITE "${alias_header_file}" "${alias_header_content}")
endforeach()

# Gather source files for compilation.
file(
    GLOB_RECURSE
    AMUSE_CORE_SOURCES
    CONFIGURE_DEPENDS
    "${AMUSE_CORE_SOURCES_DIR}/*.c"
    "${AMUSE_CORE_SOURCES_DIR}/*.cpp"
)

# Prefer an object library when we have real sources; otherwise expose headers only.
if (AMUSE_CORE_SOURCES)
    add_library(amuse_core OBJECT)

    target_sources(amuse_core
        PRIVATE
            ${AMUSE_CORE_SOURCES}
        PUBLIC
            ${AMUSE_CORE_HEADERS}
    )

    target_include_directories(amuse_core
        PUBLIC
            $<BUILD_INTERFACE:${AMUSE_CORE_HEADERS_DIR}>
            $<BUILD_INTERFACE:${AMUSE_RUNTIME_GENERATED_INCLUDE_DIR}>
    )
else()
    add_library(amuse_core INTERFACE)

    target_include_directories(amuse_core
        INTERFACE
            $<BUILD_INTERFACE:${AMUSE_CORE_HEADERS_DIR}>
            $<BUILD_INTERFACE:${AMUSE_RUNTIME_GENERATED_INCLUDE_DIR}>
    )
endif()
