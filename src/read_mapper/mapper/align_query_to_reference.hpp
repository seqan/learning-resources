
#pragma once

#include <seqan3/alignment/matrix/detail/aligned_sequence_builder.hpp>
#include <seqan3/alphabet/nucleotide/dna4.hpp>

namespace read_mapper::mapper
{

using alignment_t = typename seqan3::detail::make_pairwise_alignment_type<std::span<seqan3::dna4 const>,
                                                                          std::span<seqan3::dna4 const>>::type;

using align_query_to_reference_callback
    = std::function<void(int const /*score*/,
                         size_t const /*sequence1_begin_position*/,
                         read_mapper::mapper::alignment_t const & /*alignment*/)>;

void align_query_to_reference(std::span<seqan3::dna4 const> query_sequence,
                              std::span<seqan3::dna4 const> reference_region,
                              align_query_to_reference_callback callback);

} // read_mapper::mapper
