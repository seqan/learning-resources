
#pragma once

#include "../app_index.hpp"

namespace read_mapper::mapper
{

using search_query_callback = std::function<void(size_t const /*reference_begin_position*/, size_t const /*reference_id*/)>;

void search_query(read_mapper::app_index const & app_index,
                  std::span<seqan3::dna4 const> const query_sequence,
                  search_query_callback callback);

} // read_mapper::mapper
