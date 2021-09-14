
#pragma once

#include <seqan3/std/filesystem>

struct indexer_cli_arguments
{
    std::filesystem::path reference_path{};
    std::filesystem::path index_path{"out.index"};

    int parse(int const argc, char const ** argv);
};
