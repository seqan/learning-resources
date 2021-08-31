# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2021, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2021, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
# -----------------------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.8)
project (learning_resources_test_unit CXX)

include (../learning_resources-test.cmake)

macro (learning_resources_test unit_test_cpp)
    file (RELATIVE_PATH unit_test "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_LIST_DIR}/${unit_test_cpp}")
    seqan3_test_component (target "${unit_test}" TARGET_NAME)
    seqan3_test_component (test_name "${unit_test}" TEST_NAME)

    add_executable (${target} ${unit_test_cpp})
    target_link_libraries (${target} learning_resources::test)
    add_test (NAME "${test_name}" COMMAND ${target})

    unset (unit_test)
    unset (target)
    unset (test_name)
endmacro ()

add_subdirectories ()
