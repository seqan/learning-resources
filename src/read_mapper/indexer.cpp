#include "app_index.hpp"
#include "indexer/cli_arguments.hpp"
#include "indexer/build_app_index.hpp"
#include "indexer/store_app_index.hpp"

int main(int argc, char const ** argv)
{
    read_mapper::indexer::cli_arguments arguments{};

    int return_code = arguments.parse(argc, argv);
    if (return_code != 0)
        return return_code;

    read_mapper::app_index app_index = build_app_index(arguments);
    read_mapper::indexer::store_app_index(arguments, std::move(app_index));

    return 0;
}
