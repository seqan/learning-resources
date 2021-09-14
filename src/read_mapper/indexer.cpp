#include <seqan3/std/filesystem>

#include <seqan3/argument_parser/all.hpp>

#include "indexer.cli_arguments.hpp"

void run_program(read_mapper::indexer::cli_arguments const & arguments)
{

}

int main(int argc, char const ** argv)
{
    read_mapper::indexer::cli_arguments arguments{};

    int return_code = arguments.parse(argc, argv);
    if (return_code != 0)
        return return_code;

    run_program(arguments);

    return 0;
}
