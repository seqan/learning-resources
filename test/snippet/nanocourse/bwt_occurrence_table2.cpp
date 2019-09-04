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

//![to_index]
size_t to_index(char const chr)
{
    switch (chr)
    {
        case '$': return 0;
        case 'i': return 1;
        case 'm': return 2;
        case 'p': return 3;
        case 's': return 4;
        default: throw std::logic_error{"There is an illegal character in your text."};
    }
}
//![to_index]

//![occurrence_table_computation]
struct occurrence_table
{
    // The list of bitvectors:
    std::vector<Bitvector> data;

    // We want that 5 bitvectors are filled depending on the bwt,
    // so let's customise the constructor of occurrence_table:
    occurrence_table(std::string const & bwt)
    {
        // resize the 5 bitvectors to the length of the bwt:
        data.resize(5, Bitvector(bwt.size()));

        // fill the bitvectors
        for (size_t i = 0; i < bwt.size(); ++i)
            data[to_index(bwt[i])].write(i, 1);

        for (Bitvector & bitv : data)
            bitv.construct(3, 6);
    }

    size_t read(char const chr, size_t const i)
    {
        return data[to_index(chr)].rank(i + 1);
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
    for (auto chr : sigma)
    {
        std::cout << chr << "  ";
        for (size_t i = 0; i < bwt.size(); ++i)
            std::cout << Occ.read(chr, i) << " ";
        std::cout << '\n';
    }
}
//![main]
