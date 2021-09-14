#include "align_query_to_reference.hpp"

namespace read_mapper::mapper
{

void align_query_to_reference(std::span<seqan3::dna4 const> query_sequence,
                              std::span<seqan3::dna4 const> reference_region,
                              align_query_to_reference_callback callback)
{
}

} // read_mapper::mapper
