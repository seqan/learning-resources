#include "search_query.hpp"

namespace read_mapper::mapper
{

void search_query(cli_arguments const & arguments,
                  read_mapper::app_index const & app_index,
                  std::span<seqan3::dna4 const> const query_sequence,
                  search_query_callback callback)
{
}

} // read_mapper::mapper
