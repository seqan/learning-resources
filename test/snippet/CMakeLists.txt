# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2021, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2021, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
# -----------------------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.8)
project (learning_resources_test_snippet CXX)

include (../learning_resources-test.cmake)

macro (learning_resources_snippet test_name_prefix snippet snippet_base_path)
    seqan3_test_component (snippet_target_name "${snippet}" TARGET_NAME)
    seqan3_test_component (snippet_test_name "${snippet}" TEST_NAME)
    seqan3_test_component (snippet_target_path "${snippet}" TARGET_PATH)
    set (target "${snippet_target_name}_snippet")

    add_executable (${target} "${snippet_base_path}/${snippet}")
    target_link_libraries (${target} learning_resources::test)
    set_target_properties(${target}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${snippet_target_path}"
    )

    add_test (NAME "${test_name_prefix}/${snippet_test_name}_snippet" COMMAND ${target})

    unset (snippet_target_name)
    unset (snippet_test_name)
    unset (snippet_target_path)
    unset (target)
endmacro ()

macro (seqan3_snippets test_name_prefix snippet_base_path)
    seqan3_test_files (snippet_files "${snippet_base_path}" "*.cpp")
    foreach (snippet ${snippet_files})
        learning_resources_snippet("${test_name_prefix}" "${snippet}" "${snippet_base_path}")
    endforeach ()
endmacro ()

include(ExternalProject)
ExternalProject_Add(
    "biocpp-data"
    URL "http://ftp.seqan.de/workshop/2021/biocpp-data.tar.gz"
    URL_HASH "SHA256=f86b421bcc4586ce6ed95513c22aba1e6d41a4777046dc7d4e32b4fbd103ea11"
    DOWNLOAD_NAME "${ARG_FILE}"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    TEST_COMMAND ""
    SOURCE_DIR "${CMAKE_BINARY_DIR}/data"
)

seqan3_snippets ("snippet" "${CMAKE_SOURCE_DIR}")
