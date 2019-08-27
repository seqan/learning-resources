#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "bitvector.hpp"

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
struct occurrence_table
{
    // The list of bitvectors:
    std::vector<Bitvector> bitvector_data;

    // We want that 5 bitvectors are filled depending on the bwt,
    // so let's customise the constructor of occurrence_table:
    occurrence_table(std::string const & bwt)
    {
        // resize the 5 bitvectors, blocks and superblocks:
        bitvector_data = std::vector<Bitvector>(5, Bitvector((bwt.size() + 63)/ 64));

        // fill the bitvectors
        for (size_t i = 0; i < bwt.size(); ++i)
        {
            switch (bwt[i])
            {
                case '$': bitvector_data[0].write(i, 1); break;
                case 'i': bitvector_data[1].write(i, 1); break;
                case 'm': bitvector_data[2].write(i, 1); break;
                case 'p': bitvector_data[3].write(i, 1); break;
                case 's': bitvector_data[4].write(i, 1); break;
                default: break;
            }
        }

        for (Bitvector & bitv : bitvector_data)
            bitv.construct(3, 12);
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

        return bitvector_data[c].rank(i + 1);
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
        for (size_t i = 1; i < bwt.size() + 1; ++i)
            std::cout << bitv.rank(i) << " ";
        std::cout << "\n";
    }
}
//![main]
