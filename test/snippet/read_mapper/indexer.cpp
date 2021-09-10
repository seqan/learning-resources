#include <seqan3/argument_parser/all.hpp>
#include <seqan3/io/sequence_file/input.hpp>
#include <seqan3/search/fm_index/bi_fm_index.hpp>

struct cmd_arguments
{
    std::filesystem::path reference_path{};
    std::filesystem::path index_path{"out.index"};
};

struct dna4_traits : seqan3::sequence_file_input_default_traits_dna
{
    using sequence_alphabet = seqan3::dna4;
};

void run_program(cmd_arguments const & arguments)
{
    std::vector<std::vector<seqan3::dna4>> sequences;
    for (auto && record : seqan3::sequence_file_input<dna4_traits>{arguments.reference_path})
        sequences.emplace_back(std::move(record.sequence()));

    seqan3::bi_fm_index index{sequences};

    std::ofstream os{arguments.index_path, std::ios::binary};
    cereal::BinaryOutputArchive oarchive{os};
    oarchive(index);
}

int main(int argc, char const ** argv)
{
    cmd_arguments arguments{};

    seqan3::argument_parser parser("Indexer", argc, argv, seqan3::update_notifications::off);
    parser.info.author = "E. coli";
    parser.info.short_description = "Creates an index over a reference.";
    parser.info.version = "1.0.0";
    parser.add_option(arguments.reference_path, 'r', "reference", "The path to the reference.",
                      seqan3::option_spec::required,
                      seqan3::input_file_validator{{"fa","fasta"}});
    parser.add_option(arguments.index_path, 'o', "output", "The output index file path.",
                      seqan3::option_spec::standard,
                      seqan3::output_file_validator{seqan3::output_file_open_options::create_new, {"index"}});

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
