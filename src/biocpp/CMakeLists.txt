cmake_minimum_required(VERSION 3.4)
project (biostec CXX)

find_package(SeqAn3 3.0.3 REQUIRED HINTS ../../submodules/seqan3/build_system/)

add_executable(hello_world hello_world.cpp)
target_link_libraries(hello_world seqan3::seqan3)

add_executable(fastq2fasta fastq2fasta.cpp)
target_link_libraries(fastq2fasta seqan3::seqan3)

add_executable(quality_filter quality_filter.cpp)
target_link_libraries(quality_filter seqan3::seqan3)
