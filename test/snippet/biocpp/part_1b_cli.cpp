#include <iostream>
#include <string_view>

#include <seqan3/io/sequence_file/all.hpp>

// Alias for C-style string.
using character_string = char const *;

int main(int const argc, character_string argv[])
{
#ifdef DATA_DIR
    (void) argc; (void) argv;

    std::string_view fastq_input_path{"" DATA_DIR "/SP1.fq"};
#else
    if (argc != 2)
        return EXIT_FAILURE;

    std::string_view fastq_input_path{argv[1]};
#endif

    seqan3::sequence_file_output{std::cout, seqan3::format_fasta{}} = seqan3::sequence_file_input{fastq_input_path};

    /* Assignment 1: Write to file
     * Extend the program to read in a second argument which is the path for the output variable.
     * Write the output to the file output.
     *
     * Solution:
     * std::string_view fasta_output_path{argv[2]};
     * seqan3::sequence_file_output{fasta_output_path} = seqan3::sequence_file_input{fastq_input_path};
     */

    return EXIT_SUCCESS;
}
