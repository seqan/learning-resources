# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2021, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2021, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
# -----------------------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.8)

include (CheckCXXSourceCompiles)
include (FindPackageHandleStandardArgs)
include (FindPackageMessage)

find_package (SeqAn3 REQUIRED PATHS "${CMAKE_CURRENT_LIST_DIR}/../submodules/seqan3/build_system/")

# ----------------------------------------------------------------------------
# Commonly shared options for external projects.
# ----------------------------------------------------------------------------

set (SEQAN3_EXTERNAL_PROJECT_CMAKE_ARGS "")
list (APPEND SEQAN3_EXTERNAL_PROJECT_CMAKE_ARGS "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
list (APPEND SEQAN3_EXTERNAL_PROJECT_CMAKE_ARGS "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
list (APPEND SEQAN3_EXTERNAL_PROJECT_CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}")
list (APPEND SEQAN3_EXTERNAL_PROJECT_CMAKE_ARGS "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}")
set (SEQAN3_TEST_CLONE_DIR "${PROJECT_BINARY_DIR}/vendor/googletest")

# ----------------------------------------------------------------------------
# Include CMake scripts from seqan3.
# ----------------------------------------------------------------------------

list(APPEND CMAKE_MODULE_PATH "${SEQAN3_CLONE_DIR}/test/cmake/")

include (add_subdirectories)
include (seqan3_require_ccache)
include (seqan3_require_test)
include (seqan3_test_component)
include (seqan3_test_files)

seqan3_require_ccache ()
seqan3_require_test ()

# ----------------------------------------------------------------------------
# Interface targets for the different tests.
# ----------------------------------------------------------------------------

add_library (learning_resources_test INTERFACE)
target_compile_options (learning_resources_test INTERFACE "-pedantic"  "-Wall" "-Wextra" "-Werror")
target_compile_definitions (learning_resources_test INTERFACE DATA_DIR="${CMAKE_BINARY_DIR}/data")
target_link_libraries (learning_resources_test INTERFACE "gtest_main" "gtest" "seqan3::seqan3" "pthread")
add_library (learning_resources::test ALIAS learning_resources_test)
