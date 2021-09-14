
#pragma once

#include <functional>
#include <seqan3/std/span>

#include <seqan3/alphabet/nucleotide/dna4.hpp>

#include "cli_arguments.hpp"

namespace read_mapper::mapper
{

using read_query_file_callback = std::function<void(std::span<seqan3::dna4 const> const /*query_sequence*/)>;

void read_query_file(cli_arguments & arguments, read_query_file_callback callback);

} // read_mapper::mapper
