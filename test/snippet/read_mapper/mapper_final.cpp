#include <seqan3/alignment/configuration/all.hpp>
#include <seqan3/alignment/pairwise/align_pairwise.hpp>
#include <seqan3/argument_parser/all.hpp>
#include <seqan3/io/sam_file/output.hpp>
#include <seqan3/io/sequence_file/input.hpp>
#include <seqan3/search/search.hpp>

#include "app_index.hpp"

struct cmd_arguments
{
    std::filesystem::path query_path{};
    std::filesystem::path index_path{};
    std::filesystem::path sam_path{"out.sam"};
    uint8_t errors{0};
};

struct dna4_traits : seqan3::sequence_file_input_default_traits_dna
{
    using sequence_alphabet = seqan3::dna4;
};

void run_program(cmd_arguments const & arguments)
{
    app_index index{};

    {
        std::ifstream is{arguments.index_path, std::ios::binary};
        cereal::BinaryInputArchive iarchive{is};
        iarchive(index);
    }

    seqan3::sequence_file_input<dna4_traits> query_file_in{arguments.query_path};

    using sam_fields_t = seqan3::fields<seqan3::field::seq,
                                        seqan3::field::id,
                                        seqan3::field::ref_id,
                                        seqan3::field::ref_offset,
                                        seqan3::field::alignment,
                                        seqan3::field::qual,
                                        seqan3::field::mapq>;

    seqan3::sam_file_output sam_file_out{arguments.sam_path, sam_fields_t{}};

    seqan3::configuration const search_config
        = seqan3::search_cfg::max_error_total{seqan3::search_cfg::error_count{arguments.errors}}
        | seqan3::search_cfg::hit_all_best{};

    seqan3::configuration const align_config
        = seqan3::align_cfg::method_global{seqan3::align_cfg::free_end_gaps_sequence1_leading{true},
                                           seqan3::align_cfg::free_end_gaps_sequence2_leading{false},
                                           seqan3::align_cfg::free_end_gaps_sequence1_trailing{true},
                                           seqan3::align_cfg::free_end_gaps_sequence2_trailing{false}}
        | seqan3::align_cfg::edit_scheme
        | seqan3::align_cfg::output_alignment{}
        | seqan3::align_cfg::output_begin_position{}
        | seqan3::align_cfg::output_score{};

    for (/*seqan3::sequence_record*/ auto && record : query_file_in)
    {
        auto & query_sequence = record.sequence();

        auto search_results = search(query_sequence, index.fm_index, search_config);

        for (seqan3::search_result result : search_results)
        {
            size_t const approximate_reference_begin_position = result.reference_begin_position();
            size_t const region_size = query_sequence.size() + arguments.errors;

            seqan3::dna4_vector const & reference_sequence = index.sequences[result.reference_id()];

            auto reference_region_begin = reference_sequence.begin() + approximate_reference_begin_position;
            // either sets region_end = begin + region_size or end depending whether region_size goes out of bounds
            auto reference_region_end = std::ranges::next(reference_region_begin,
                                                          region_size,
                                                          reference_sequence.end());

            // conceptional the same as: std::pair<seqan3::dna4 const *, seqan3::dna4 const *>
            std::span<seqan3::dna4 const> reference_region{reference_region_begin, reference_region_end};

            auto alignments = seqan3::align_pairwise(std::tie(reference_region, query_sequence), align_config);

            for (/*seqan3::alignment_result*/ auto && alignment : alignments)
            {
                size_t const reference_position = approximate_reference_begin_position +
                                                  alignment.sequence1_begin_position();
                size_t const mapping_quality = 60u + alignment.score();

                sam_file_out.emplace_back(query_sequence,
                                          record.id(),
                                          index.ids[result.reference_id()],
                                          reference_position,
                                          alignment.alignment(),
                                          record.base_qualities(),
                                          mapping_quality);
            }
        }
    }
}

void initialise_argument_parser(seqan3::argument_parser & parser, cmd_arguments & arguments)
{
    parser.info.author = "E. coli";
    parser.info.short_description = "Map reads against a reference.";
    parser.info.version = "1.0.0";
    parser.add_option(arguments.query_path, 'q', "query", "The path to the query.",
                      seqan3::option_spec::required,
                      seqan3::input_file_validator{{"fq","fastq"}});
    parser.add_option(arguments.index_path, 'i', "index", "The path to the index.",
                      seqan3::option_spec::required,
                      seqan3::input_file_validator{{"index"}});
    parser.add_option(arguments.sam_path, 'o', "output", "The output SAM file path.",
                      seqan3::option_spec::standard,
                      seqan3::output_file_validator{seqan3::output_file_open_options::create_new, {"sam"}});
    parser.add_option(arguments.errors, 'e', "error", "Maximum allowed errors.",
                      seqan3::option_spec::standard,
                      seqan3::arithmetic_range_validator{0, 4});
}

int main(int argc, char const ** argv)
{
    seqan3::argument_parser parser("Mapper", argc, argv, seqan3::update_notifications::off);
    cmd_arguments arguments{};

    initialise_argument_parser(parser, arguments);

    try
    {
        parser.parse();
    }
    catch (seqan3::argument_parser_error const & ext)
    {
        std::cerr << "[PARSER ERROR] " << ext.what() << '\n';
        return -1;
    }

    run_program(arguments);

    return 0;
}
