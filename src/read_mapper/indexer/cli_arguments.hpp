
#pragma once

#include <seqan3/std/filesystem>

namespace read_mapper::indexer
{

struct cli_arguments
{
    std::filesystem::path reference_path{};
    std::filesystem::path output_path{"out.index"};

    int parse(int const argc, char const ** argv);
};

} // read_mapper::indexer
