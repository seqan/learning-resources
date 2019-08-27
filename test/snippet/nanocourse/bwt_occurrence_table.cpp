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

struct occurrence_table
{
    // The list of bitvectors:
    std::vector<bitvector> bitvector_data;

    // We want that 5 bitvectors are filled depending on the bwt,
    // so let's customise the constructor of occurrence_table:
    occurrence_table(std::string const & bwt)
    {
        // resize the 5 bitvectors to the length of the bwt:
        bitvector_data = std::vector<bitvector>(5, bitvector((bwt.size() + 63)/ 64));

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
    occurrence_table Occ(bwt); // construction with bwt as the parameter

    std::cout << "   ";
    for (auto c : bwt)
        std::cout << c << " ";
    std::cout << "\n   -----------------------\n";

    std::string sigma{"$imps"};
    size_t s{};
    for (auto & bitv : Occ.bitvector_data)
    {
        std::cout << sigma[s++] << "  ";
        for (size_t i = 0; i < bwt.size(); ++i)
            std::cout << read(bitv, i) << " ";
        std::cout << "\n";
    }
}
//![main]
