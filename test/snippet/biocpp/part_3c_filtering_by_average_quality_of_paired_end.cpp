#include <array>
#include <seqan3/std/concepts>
#include <seqan3/std/ranges>
#include <string_view>

#include <seqan3/alphabet/quality/all.hpp>
#include <seqan3/io/sequence_file/all.hpp>
#include <seqan3/alphabet/views/to_rank.hpp>
#include <seqan3/utility/views/join_with.hpp>
#include <seqan3/utility/views/zip.hpp>

#include <range/v3/view/concat.hpp>

template <std::ranges::input_range range_t>
    requires std::integral<std::ranges::range_value_t<range_t>>
int32_t sum(range_t && range)
{
    int32_t intermediate_sum{};
    for (auto && element : range)
        intermediate_sum += element;

    return intermediate_sum;
}

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

    std::string_view fastq_input_path1{"" DATA_DIR "/SRR1770413_1_40k.fastq"};
    std::string_view fastq_input_path2{"" DATA_DIR "/SRR1770413_2_40k.fastq"};
    std::string_view fasta_output_path{"" DATA_DIR "/SRR1770413_2_40k.solution3c.fa"};

    seqan3::phred42 minimum_phred_quality = seqan3::phred42{}.assign_rank(20);
#else
    if (argc != 4)
        return EXIT_FAILURE;

    std::string_view fastq_input_path1{argv[1]};
    std::string_view fastq_input_path2{argv[2]};
    std::string_view fasta_output_path{argv[3]};

    seqan3::phred42 minimum_phred_quality = read_user_base_quality();
#endif

    // Find average quality.
    auto by_average_quality = [&] (auto && fastq_record)
    {
        auto quality_rank_view = fastq_record.base_qualities() | seqan3::views::to_rank;
        seqan3::phred42 average_phred_quality{};
        average_phred_quality.assign_rank(sum(quality_rank_view) / std::ranges::distance(quality_rank_view));
        return average_phred_quality > minimum_phred_quality;
    };

    seqan3::sequence_file_input seq_file_in1{fastq_input_path1};
    seqan3::sequence_file_input seq_file_in2{fastq_input_path2};
    seqan3::sequence_file_output seq_file_out{fasta_output_path};

    // Only print sequences with average quality filter.
#if SEQAN3_WORKAROUND_GCC_96070 // fixed since gcc10.4
    std::vector<std::ranges::range_value_t<decltype(seq_file_in1)>> seq_file_in1_{};
    std::vector<std::ranges::range_value_t<decltype(seq_file_in2)>> seq_file_in2_{};

    // TODO: this has a different issue with input_iterator
    std::ranges::copy(seq_file_in1, std::cpp20::back_inserter(seq_file_in1_));
    std::ranges::copy(seq_file_in2, std::cpp20::back_inserter(seq_file_in2_));
    auto filtered_paired_ends = seqan3::views::zip(seq_file_in1_, seq_file_in2_)
#else
    auto filtered_paired_ends = seqan3::views::zip(seq_file_in1, seq_file_in2)
#endif
                              | std::views::filter([&] (auto && fastq_record_pair)
                                 {
                                     auto && [left_read, right_read] = fastq_record_pair;
                                     return by_average_quality(left_read) && by_average_quality(right_read);
                                 });

    for (auto && [left_read, right_read]: filtered_paired_ends)
    {
        seq_file_out.push_back(left_read);
        seq_file_out.push_back(right_read);
    }

    return EXIT_SUCCESS;
}
