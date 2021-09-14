
#pragma once

#include <seqan3/std/filesystem>

namespace read_mapper::mapper
{

struct cli_arguments
{
    std::filesystem::path query_path{};
    std::filesystem::path index_path{};
    std::filesystem::path output_path{"out.sam"};
    uint8_t errors{0u};

    int parse(int const argc, char const ** argv);
};

} // read_mapper::indexer
