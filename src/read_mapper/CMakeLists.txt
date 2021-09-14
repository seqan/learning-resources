cmake_minimum_required(VERSION 3.4)
project (biostec CXX)

find_package(SeqAn3 3.0.3 REQUIRED HINTS ../../submodules/seqan3/build_system/)

add_executable(indexer
               indexer.cpp
               indexer.cli_arguments.cpp
               indexer.build_app_index.cpp
               indexer.store_app_index.cpp)
target_link_libraries(indexer seqan3::seqan3)

add_executable(mapper
               mapper.cpp
               mapper/cli_arguments.cpp
               mapper/read_app_index_file.cpp
               mapper/read_query_file.cpp
               mapper/search_query.cpp
               mapper/align_query_to_reference.cpp)
target_link_libraries(mapper seqan3::seqan3)
