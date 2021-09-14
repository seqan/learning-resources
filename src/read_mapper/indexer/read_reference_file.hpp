
#pragma once

#include "../app_index.hpp"
#include "cli_arguments.hpp"

namespace read_mapper::indexer
{

app_index read_reference_file(read_mapper::indexer::cli_arguments const & arguments);

} // read_mapper::indexer
