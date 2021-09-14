#include <vector>

#include <seqan3/alphabet/nucleotide/dna4.hpp>
#include <seqan3/search/fm_index/bi_fm_index.hpp>

#pragma once

struct app_index
{
    std::vector<std::string> ids;
    std::vector<seqan3::dna4_vector> sequences;
    seqan3::bi_fm_index<seqan3::dna4, seqan3::text_layout::collection> fm_index;

    template <typename archive_t>
    void serialize(archive_t & archive)
    {
        archive(ids);
        archive(sequences);
        archive(fm_index);
    }
};
