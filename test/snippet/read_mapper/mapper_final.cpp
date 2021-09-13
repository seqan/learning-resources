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

struct reference_store_t
{
    std::vector<std::string> ids;
    std::vector<std::vector<seqan3::dna4>> sequences;

    template <typename archive_t>
    void serialize(archive_t & archive)
    {
        archive(ids);
        archive(sequences);
    }
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

    namespace sc = seqan3::search_cfg;
    seqan3::configuration const search_config = sc::max_error_total{sc::error_count{arguments.errors}}
                                              | sc::hit_all_best{};

    namespace ac = seqan3::align_cfg;
    seqan3::configuration const align_config = ac::method_global{ac::free_end_gaps_sequence1_leading{true},
                                                                 ac::free_end_gaps_sequence2_leading{false},
                                                                 ac::free_end_gaps_sequence1_trailing{true},
                                                                 ac::free_end_gaps_sequence2_trailing{false}}
                                             | ac::edit_scheme
                                             | ac::output_alignment{}
                                             | ac::output_begin_position{}
                                             | ac::output_score{};

    for (/*seqan3::sequence_record*/ auto && record : query_file_in)
    {
        auto & query = record.sequence();

        auto search_results = search(query, index.fm_index, search_config);

        for (seqan3::search_result result : search_results)
        {
            size_t const start = result.reference_begin_position();
            auto reference_begin = index.sequences[result.reference_id()].begin() + start;
            auto reference_end = std::ranges::next(reference_begin,
                                                   query.size() + arguments.errors,
                                                   index.sequences[result.reference_id()].end());
            std::span reference_view{reference_begin, reference_end};

            auto alignments = seqan3::align_pairwise(std::tie(reference_view, query), align_config);

            for (/*seqan3::alignment_result*/ auto && alignment : alignments)
            {
                auto const aligned_seq = alignment.alignment();
                size_t const ref_offset = alignment.sequence1_begin_position() + start;
                size_t const map_qual = 60u + alignment.score();

                sam_file_out.emplace_back(query,
                                          record.id(),
                                          index.ids[result.reference_id()],
                                          ref_offset,
                                          aligned_seq,
                                          record.base_qualities(),
                                          map_qual);
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
