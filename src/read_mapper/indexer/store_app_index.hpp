
#pragma once

#include "../app_index.hpp"
#include "cli_arguments.hpp"

namespace read_mapper::indexer
{

void store_app_index(read_mapper::indexer::cli_arguments const & arguments, app_index const & app_index);

} // read_mapper::indexer
