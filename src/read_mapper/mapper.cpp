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

    read_mapper::app_index app_index = read_mapper::mapper::read_app_index_file(arguments);

    read_mapper::mapper::read_query_file(arguments, [&](std::span<seqan3::dna4 const> const query_sequence) -> void
    {
        read_mapper::mapper::search_query(app_index, query_sequence, [&](size_t const reference_begin_position,
                                                                         size_t const reference_id) -> void
        {
            std::span<seqan3::dna4 const> const reference_region{};

            read_mapper::mapper::align_query_to_reference(query_sequence,
                                                          reference_region,
                                                          [&](int const score,
                                                              size_t const sequence1_begin_position,
                                                              read_mapper::mapper::alignment_t const & alignment)
            {
            });
        });
    });

    return 0;
}
