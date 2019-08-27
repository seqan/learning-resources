#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

//![bwt_construction]
std::string bwt_construction(std::string const & text)
{
    std::string bwt{};

    std::vector<uint64_t> sa(text.size() + 1);
    std::iota(sa.begin(), sa.end(), 0);
    std::sort(sa.begin(), sa.end(), [&text](uint64_t a, uint64_t b) -> bool
    {
        while (a < text.size() && b < text.size() && text[a] == text[b])
        {
            ++a, ++b;
        }

        if (b == text.size())
            return false;

        if (a == text.size())
            return true;

        return text[a] < text[b];
    });

    for (auto x : sa)
    {
        if (!x)
            bwt += "$";
        else
            bwt += text[x-1];
    }

    return bwt;
}
//![bwt_construction]

//![occurrence_table_computation]

// Lets's rename the type `std::vector<uint64_t>` to `bitvector` for a more readable code:
using bitvector = std::vector<uint64_t>;
using block = std::vector<uint16_t>;
using superblock = std::vector<uint64_t>;

void write(bitvector & B, size_t const i, bool const x)
{
    uint64_t mask = static_cast<uint64_t>(1) << (63 - (i % 64));

    if (x == 1)
        B[i / 64] |= mask;
    else
        B[i / 64] &= ~mask;
}

bool read(bitvector const & B, size_t const i)
{
    return (B[i / 64] >> (63 - (i % 64))) & 1;
}


void construct(block & blocks, superblock & superblocks, bitvector const & B)
{
    size_t block_pos{0u};
    size_t super_block_pos{0u};

    uint16_t block_count{0u};
    uint64_t super_block_count{0u};

    for (size_t i = 0u; i < B.size() * 64; ++i)
    {
        if (i % 3 == 0u)
        {
            if (i % 6 == 0u)
            {
                super_block_count += block_count; // update superblock count

                superblocks[super_block_pos] = super_block_count;

                ++super_block_pos; // move to the next position
                block_count = 0u;   // reset block count
            }

            blocks[block_pos] = block_count;

            ++block_pos; // move to the next position
        }

        if (read(B, i) == true)
            ++block_count;
    }
}

uint64_t rank(bitvector const & B, block const & blocks, superblock const & superblocks, size_t const i)
{
    uint64_t rank{0};

    rank += superblocks[(i - 1) / 1600];
    rank += blocks[(i - 1) / 64];

    for (size_t j = ((i - 1) / 64) * 64; j < i; ++j)
    {
        rank += read(B, i);
    }

    return rank;
}

struct occurrence_table
{
    // The list of bitvectors:
    std::vector<bitvector> bitvector_data;
    // The list of blocks:
    std::vector<block> block_data;
    // The list of superblocks:
    std::vector<superblock> superblock_data;

    // We want that 5 bitvectors are filled depending on the bwt,
    // so let's customise the constructor of occurrence_table:
    occurrence_table(std::string const & bwt)
    {
        // resize the 5 bitvectors, blocks and superblocks:
        bitvector_data = std::vector<bitvector>(5, bitvector((bwt.size() + 63)/ 64));
        block_data = std::vector<block>(5, block((bwt.size() + 2)/ 3));
        superblock_data = std::vector<superblock>(5, superblock((bwt.size() + 5) / 6));

        // fill the bitvectors
        for (size_t i = 0; i < bwt.size(); ++i)
        {
            switch (bwt[i])
            {
                case '$': write(bitvector_data[0], i, 1); break;
                case 'i': write(bitvector_data[1], i, 1); break;
                case 'm': write(bitvector_data[2], i, 1); break;
                case 'p': write(bitvector_data[3], i, 1); break;
                case 's': write(bitvector_data[4], i, 1); break;
                default: break;
            }
        }

        for (size_t i = 0; i < 5; ++i)
            construct(block_data[i], superblock_data[i], bitvector_data[i]);
    }

    bool read(char const chr, size_t const i)
    {
        size_t c{}; // the index of the character

        switch (chr)
        {
            case '$': c = 0; break;
            case 'i': c = 1; break;
            case 'm': c = 2; break;
            case 'p': c = 3; break;
            case 's': c = 4; break;
            default: break;
        }

        return rank(bitvector_data[c], block_data[c], superblock_data[c], i + 1);
    }
};


//![occurrence_table_computation]

//![main]
int main()
{
    std::string text{"mississippi"};
    std::string bwt = bwt_construction(text);

    // occurrence table
    // -------------------------------------------------------------------------
    // occurrence_table Occ(bwt); // construction with bwt as the parameter

    // std::cout << Occ.read('s', 2) << "\n";
    // std::cout << Occ.read('s', 5) << "\n";
    // std::cout << Occ.read('p', 9) << "\n";
}
//![main]
