#include <seqan3/argument_parser/all.hpp>

#include "cli_arguments.hpp"

namespace read_mapper::mapper
{

int cli_arguments::parse(int const argc, char const ** argv)
{
    seqan3::argument_parser parser("Mapper", argc, argv, seqan3::update_notifications::off);

    parser.info.author = "E. coli";
    parser.info.short_description = "Map reads against a reference.";
    parser.info.version = "1.0.0";
    parser.add_option(this->query_path, 'q', "query", "The path to the query.",
                      seqan3::option_spec::required,
                      seqan3::input_file_validator{{"fq","fastq"}});
    parser.add_option(this->index_path, 'i', "index", "The path to the index.",
                      seqan3::option_spec::required,
                      seqan3::input_file_validator{{"index"}});
    parser.add_option(this->output_path, 'o', "output", "The output SAM file path.",
                      seqan3::option_spec::standard,
                      seqan3::output_file_validator{seqan3::output_file_open_options::create_new, {"sam"}});
    parser.add_option(this->error, 'e', "error", "Maximum allowed errors.",
                      seqan3::option_spec::standard,
                      seqan3::arithmetic_range_validator{0, 4});

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
