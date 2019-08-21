# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2019, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2019, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
# -----------------------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.7)
project (lr_test_unit CXX)

include (../ci.cmake)

macro (lr_test unit_test_cpp)
    file (RELATIVE_PATH unit_test "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_LIST_DIR}/${unit_test_cpp}")
    lr_test_component (target "${unit_test}" TARGET_NAME)
    lr_test_component (test_name "${unit_test}" TEST_NAME)

    add_executable (${target} ${unit_test_cpp})
    target_link_libraries (${target} lr::test::unit)
    add_test (NAME "${test_name}" COMMAND ${target})

    unset (unit_test)
    unset (target)
    unset (test_name)
endmacro ()

lr_require_ccache ()
lr_require_test ()

add_subdirectories ()
