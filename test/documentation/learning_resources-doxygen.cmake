# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2021, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2021, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
# -----------------------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.8)

### Find doxygen and dependency to DOT tool
message (STATUS "Searching for doxygen.")
find_package (Doxygen REQUIRED)

if (NOT ${DOXYGEN_FOUND})
    message (FATAL_ERROR "Could not find doxygen. Not building documentation.")
endif ()

if (NOT ${DOXYGEN_DOT_FOUND})
    message (STATUS "Could not find dot tool. Disabling dot support.")
    set (LEARNING_RESOURCES_DOXYGEN_HAVE_DOT "NO")
else ()
    message (STATUS "Found dot tool. Enabling dot support.")
    set (LEARNING_RESOURCES_DOXYGEN_HAVE_DOT "YES")
endif ()

### Configure doc/developer targets.
set(LEARNING_RESOURCES_DOXYFILE_IN ${LEARNING_RESOURCES_DOXYGEN_INPUT_DIR}/learning_resources_doxygen_cfg.in)

message (STATUS "Configuring user doc.")

set (LEARNING_RESOURCES_DOXYGEN_OUTPUT_DIR "${PROJECT_BINARY_DIR}/doc")
set (LEARNING_RESOURCES_DOXYGEN_SOURCE_DIR "${LEARNING_RESOURCES_DOXYGEN_INPUT_DIR}/../..")
set (LEARNING_RESOURCES_DOXYGEN_ENABLED_SECTIONS "")
set (LEARNING_RESOURCES_DOXYGEN_EXTRACT_PRIVATE "NO")

configure_file (${LEARNING_RESOURCES_DOXYFILE_IN} ${LEARNING_RESOURCES_DOXYGEN_OUTPUT_DIR}/Doxyfile)

add_custom_target(doc ALL
                  COMMAND ${DOXYGEN_EXECUTABLE}
                  WORKING_DIRECTORY ${LEARNING_RESOURCES_DOXYGEN_OUTPUT_DIR}
                  COMMENT "Generating user API documentation with Doxygen"
                  VERBATIM)
