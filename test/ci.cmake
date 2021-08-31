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
# Include ccache and googletest scripts from seqan3.
# ----------------------------------------------------------------------------

include ("${SEQAN3_CLONE_DIR}/test/cmake/seqan3_require_ccache.cmake")
include ("${SEQAN3_CLONE_DIR}/test/cmake/seqan3_require_test.cmake")

seqan3_require_ccache ()
seqan3_require_test ()

# ----------------------------------------------------------------------------
# Interface targets for the different test modules in seqan3.
# ----------------------------------------------------------------------------

# seqan3::test exposes a base set of required flags, includes, definitions and
# libraries which are in common for **all** seqan3 tests
add_library (lr_test INTERFACE)
target_compile_options (lr_test INTERFACE "-pedantic"  "-Wall" "-Wextra" "-Werror")
target_link_libraries (lr_test INTERFACE "pthread")
add_library (lr::test ALIAS lr_test)

# seqan3::test::unit specifies required flags, includes and libraries
# needed for unit test cases in seqan3/test/unit
add_library (lr_test_unit INTERFACE)
target_link_libraries (lr_test_unit INTERFACE "gtest_main" "gtest" "lr::test" "seqan3::seqan3")
add_library (lr::test::unit ALIAS lr_test_unit)

# ----------------------------------------------------------------------------
# Commonly used macros for the different test modules in seqan3.
# ----------------------------------------------------------------------------

# Get a specific component of a test file which follows the seqan3 naming scheme.
# e.g. target_source_file = "range/view/take.cpp"
# component:
#  * TARGET_NAME - the target name (e.g. "take")
#  * TARGET_UNIQUE_NAME - the target name which includes the target_path (e.g. range-view-take)
#  * TEST_NAME - the test name which includes the target_path (e.g. "range/view/take")
#  * TARGET_PATH - the path to the target source (e.g. "range/view")
macro (lr_test_component VAR target_source_file component_name_)
    string (TOUPPER "${component_name_}" component_name)

    get_filename_component (target_relative_path "${target_source_file}" DIRECTORY)
    get_filename_component (target_name "${target_source_file}" NAME_WE)

    if (component_name STREQUAL "TARGET_NAME")
        set (${VAR} "${target_name}")
    elseif (component_name MATCHES "TEST_NAME|TARGET_UNIQUE_NAME")
        if (target_relative_path)
            set (${VAR} "${target_relative_path}/${target_name}")
        else ()
            set (${VAR} "${target_name}")
        endif ()

        if (component_name STREQUAL "TARGET_UNIQUE_NAME")
            string (REPLACE "/" "-" ${VAR} "${${VAR}}")
        endif ()
    elseif (component_name STREQUAL "TARGET_PATH")
        set (${VAR} "${target_relative_path}")
    endif ()

    unset (target_name)
    unset (target_relative_path)
endmacro ()

macro (lr_test_files VAR test_base_path_ extension_wildcards)
    # test_base_path is /home/.../seqan3/test/
    get_filename_component(test_base_path "${test_base_path_}" ABSOLUTE)
    file (RELATIVE_PATH test_base_path_relative "${CMAKE_SOURCE_DIR}" "${test_base_path}")
    # ./ is a hack to deal with empty test_base_path_relative
    set (test_base_path_relative "./${test_base_path_relative}")
    # collect all cpp files
    set (${VAR} "")
    foreach (extension_wildcard ${extension_wildcards})
        file (GLOB_RECURSE test_files RELATIVE "${test_base_path}"
        "${test_base_path_relative}/${extension_wildcard}")
        list (APPEND ${VAR} ${test_files})
    endforeach ()

    unset (test_base_path)
    unset (test_base_path_relative)
endmacro ()

macro (add_subdirectories_of directory)
    file (GLOB ENTRIES
          RELATIVE ${directory}
          ${directory}/[!.]*)

    foreach (ENTRY ${ENTRIES})
        if (IS_DIRECTORY ${directory}/${ENTRY})
            if (EXISTS ${directory}/${ENTRY}/CMakeLists.txt)
                add_subdirectory (${directory}/${ENTRY} ${CMAKE_CURRENT_BINARY_DIR}/${ENTRY})
            endif ()
        endif ()
    endforeach ()
    unset (ENTRIES)
endmacro ()

macro (add_subdirectories)
    add_subdirectories_of(${CMAKE_CURRENT_SOURCE_DIR})
endmacro ()
