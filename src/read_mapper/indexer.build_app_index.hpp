
#pragma once

#include "app_index.hpp"
#include "indexer.cli_arguments.hpp"

namespace read_mapper::indexer
{

app_index build_app_index(read_mapper::indexer::cli_arguments const & arguments);

} // read_mapper::indexer
