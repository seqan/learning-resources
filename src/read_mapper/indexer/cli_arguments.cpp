#include <seqan3/argument_parser/all.hpp>

#include "cli_arguments.hpp"

namespace read_mapper::indexer
{

int cli_arguments::parse(int const argc, char const ** argv)
{
    seqan3::argument_parser parser("Indexer", argc, argv, seqan3::update_notifications::off);

    parser.info.author = "E. coli";
    parser.info.short_description = "Creates an index over a reference.";
    parser.info.version = "1.0.0";
    parser.add_option(this->reference_path, 'r', "reference", "The path to the reference.",
                      seqan3::option_spec::required,
                      seqan3::input_file_validator{{"fa","fasta"}});
    parser.add_option(this->output_path, 'o', "output", "The output index file path.",
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

    return 0;
}

} // read_mapper::indexer
