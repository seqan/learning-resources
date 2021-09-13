cmake_minimum_required(VERSION 3.4)
project (biostec CXX)

find_package(SeqAn3 3.0.3 REQUIRED HINTS ../../submodules/seqan3/build_system/)

add_executable(indexer indexer.cpp)
target_link_libraries(indexer seqan3::seqan3)

add_executable(mapper mapper.cpp)
target_link_libraries(mapper seqan3::seqan3)
