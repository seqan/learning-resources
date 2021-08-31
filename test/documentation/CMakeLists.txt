# Minimum cmake version
cmake_minimum_required(VERSION 3.7)

# Name of project
project(learning_resources_documentation NONE)

set (LEARNING_RESOURCES_DOXYGEN_INPUT_DIR "${CMAKE_SOURCE_DIR}")

include (learning_resources-doxygen.cmake)

### Find python interpreter
find_package(PythonInterp REQUIRED)

### Enable testing
enable_testing()

set (LEARNING_RESOURCES_DOXYGEN_TEST_SCRIPT "
    output=\$(${DOXYGEN_EXECUTABLE} 2>&1 > /dev/null | sort);
    echo \"\$output\";
    test -z \"\$output\"")

message (STATUS "Adding doc test.")
add_test(NAME doc_test
COMMAND sh -c "${LEARNING_RESOURCES_DOXYGEN_TEST_SCRIPT}"
WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/doc)
