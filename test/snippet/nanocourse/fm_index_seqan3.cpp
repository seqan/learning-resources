#include <seqan3/std/filesystem>
#include <fstream>

#include <seqan3/core/debug_stream.hpp>

// write temporary file s.t. the snippet compiles
std::filesystem::path const file_path_to_reference = std::filesystem::temp_directory_path()/"my.fastq";
std::filesystem::path const & file_path_to_query = file_path_to_reference;

struct write_file_dummy_struct
{

    write_file_dummy_struct()
    {

auto file_raw = R"//![fasta_file](
@seq1
AGCTAGCAGCGATCG
+
IIIIIHIIIIIIIII
@seq2
CGATCGATC
+
IIIIIIIII
@seq3
AGCGATCGAGGAATATAT
+
IIIIHHGIIIIHHGIIIH
)//![fasta_file]";

        std::ofstream file{file_path_to_reference};
        std::string str{file_raw};
        file << str.substr(1); // skip first newline
    }

    ~write_file_dummy_struct()
    {
        std::error_code ec{};
        std::filesystem::remove(file_path_to_reference, ec);

        if (ec)
            seqan3::debug_stream << "Could not delete " << file_path_to_reference << ". " << ec.message() << '\n';
    }
};

write_file_dummy_struct go{};

//![code]
#include <iostream>

#include <seqan3/io/sequence_file/all.hpp>
#include <seqan3/search/all.hpp>

int main()
{
    seqan3::sequence_file_input reference_in{file_path_to_reference}; // Read the reference (only 1 contig).
    seqan3::sequence_file_input query_in{file_path_to_reference}; // Read the query file (multiple contigs).

     // Create an FM-index from the reference (first record).
    seqan3::fm_index index{(*reference_in.begin()).sequence()};

    size_t counter{0u};

    for (auto & rec : query_in) // For each record in our query file
    {
        // search the query sequence in the index with seqan3::search
        auto res = seqan3::search(rec.sequence(), index);

        if (std::ranges::distance(res) > 100) // if more than 100 hits were found
            ++counter;                        // increase the counter
    }

    seqan3::debug_stream << "Found " << counter << " queries with more than 100 exact hits.\n";
}
//![code]
