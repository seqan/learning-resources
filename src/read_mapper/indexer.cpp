#include "app_index.hpp"
#include "indexer/cli_arguments.hpp"
#include "indexer/read_reference_file.hpp"
#include "indexer/build_fm_index.hpp"
#include "indexer/store_app_index.hpp"

int main(int argc, char const ** argv)
{
    read_mapper::indexer::cli_arguments arguments{};

    int return_code = arguments.parse(argc, argv);
    if (return_code != 0)
        return return_code;

    read_mapper::app_index app_index = read_mapper::indexer::read_reference_file(arguments);
    read_mapper::indexer::build_fm_index(app_index);
    read_mapper::indexer::store_app_index(arguments, app_index);

    return 0;
}
