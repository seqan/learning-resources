#include <seqan3/io/record.hpp>

#include "app_index.hpp"
#include "mapper/cli_arguments.hpp"
#include "mapper/read_app_index_file.hpp"
#include "mapper/read_query_file.hpp"
#include "mapper/search_query.hpp"
#include "mapper/align_query_to_reference.hpp"

int main(int argc, char const ** argv)
{
    read_mapper::mapper::cli_arguments arguments{};

    int return_code = arguments.parse(argc, argv);
    if (return_code != 0)
        return return_code;

    read_mapper::app_index const app_index = read_mapper::mapper::read_app_index_file(arguments);

    read_mapper::mapper::read_query_file(arguments, [&](std::string const & query_id,
                                                        std::span<seqan3::dna4 const> const query_sequence) -> void
    {
        read_mapper::mapper::search_query(arguments, app_index, query_sequence, [&](size_t const reference_begin_position,
                                                                                    size_t const reference_index) -> void
        {
            std::span<seqan3::dna4 const> const reference_sequence_region{};

            read_mapper::mapper::align_query_to_reference(query_sequence,
                                                          reference_sequence_region,
                                                          [&](int const score,
                                                              size_t const sequence1_begin_position,
                                                              read_mapper::mapper::alignment_t const & alignment)
            {
                using query_sequence_t = std::span<seqan3::dna4 const> const;
                using query_id_t = std::string const &;
                using reference_id_t = std::string const &;
                using mapped_position_t = size_t;
                using alignment_t = read_mapper::mapper::alignment_t const &;
                using mapping_quality_t = uint8_t;

                using field_types = seqan3::type_list<query_sequence_t,
                                                      query_id_t,
                                                      reference_id_t,
                                                      mapped_position_t,
                                                      alignment_t,
                                                      mapping_quality_t>;

                using field_ids = seqan3::fields<seqan3::field::seq,
                                                 seqan3::field::id,
                                                 seqan3::field::ref_id,
                                                 seqan3::field::ref_offset,
                                                 seqan3::field::alignment,
                                                 seqan3::field::mapq>;
            });
        });
    });

    return 0;
}
