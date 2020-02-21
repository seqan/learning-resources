#include <string_view>

#include <seqan3/alphabet/quality/all.hpp>
#include <seqan3/io/sequence_file/all.hpp>
#include <seqan3/std/algorithm>

// Request user interaction to provide the minimum base quality.
seqan3::phred42 read_user_base_quality()
{
    std::cout << "Please specify the minimum base quality as a probability between 0 and 41\n";
    int32_t user_base_quality{};
    std::cin >> user_base_quality;

    if (user_base_quality < 0 || user_base_quality > 41)
        throw std::invalid_argument{"Only values in the interval [0, 41] can be used."};

    return seqan3::phred42{}.assign_rank(user_base_quality);
}

using character_string = const char *;

int main(int const argc, character_string argv[])
{
#ifdef DATA_DIR
    (void) argc; (void) argv;

    std::string_view fastq_input_path{"" DATA_DIR "/SP1.fq"};
    std::string_view fasta_output_path{"" DATA_DIR "/SP1.solution2a.fa"};

    seqan3::phred42 minimum_phred_quality = seqan3::phred42{}.assign_rank(20);
#else
    if (argc != 3)
        return EXIT_FAILURE;

    std::string_view fastq_input_path{argv[1]};
    std::string_view fasta_output_path{argv[2]};

    seqan3::phred42 minimum_phred_quality = read_user_base_quality();
#endif

    seqan3::sequence_file_input seq_file_in{fastq_input_path};
    seqan3::sequence_file_output seq_file_out{fasta_output_path};

    // Only write out records that have no base call below the given minimum base quality.
    for (auto && [seq, id, qual] : seq_file_in) // untie elements of a tuple
    {
        if (std::ranges::all_of(qual, [&] (auto && current_quality) { return current_quality >= minimum_phred_quality; }))
            seq_file_out.emplace_back(seq, id);
    }

    return EXIT_SUCCESS;
}
