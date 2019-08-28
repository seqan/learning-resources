#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "bitvector.hpp"

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

std::vector<uint16_t> compute_count_table(std::string const & bwt)
{
    std::vector<uint16_t> count_table(5);

    for (auto chr : bwt)
    {
        for (size_t i = to_index(chr) + 1; i < 5; ++i)
            ++count_table[i];
    }

    return count_table;
}

struct occurrence_table
{
    // The list of bitvectors:
    std::vector<Bitvector> data;

    // We want that 5 bitvectors are filled depending on the bwt,
    // so let's customise the constructor of occurrence_table:
    occurrence_table(std::string const & bwt)
    {
        // resize the 5 bitvectors to the length of the bwt:
        data.resize(5, Bitvector((bwt.size() + 63)/ 64));

        // fill the bitvectors
        for (size_t i = 0; i < bwt.size(); ++i)
            data[to_index(bwt[i])].write(i, 1);

        for (Bitvector & bitv : data)
            bitv.construct(3, 6);
    }

    size_t read(char const chr, int const i)
    {
        if (i < 0)
            return 0;

        return data[to_index(chr)].rank(i + 1);
    }
};

size_t count(std::string const & P, std::string const & bwt, std::vector<uint16_t> C, occurrence_table Occ)
{
    int i = P.size() - 1;
    int a = 0;
    int b = bwt.size() - 1;

    while ((a <= b) && (i >= 0))
    {
        char c = P[i];
        a = C[to_index(c)] + Occ.read(c, a - 1);
        b = C[to_index(c)] + Occ.read(c, b) - 1;

        i = i - 1;
    }

    if (b < a)
        return 0;
    else
        return (b - a + 1);
}

//![main]
int main()
{
    std::string text{"mississippi"};

    // compute the bwt, C and Occ:
    std::string bwt = bwt_construction(text);
    std::vector<uint16_t> C = compute_count_table(bwt);
    occurrence_table Occ(bwt);

    std::cout << count("ssi", bwt, C, Occ) << '\n'; // prints 2
    std::cout << count("ppi", bwt, C, Occ) << '\n'; // prints 1
}
//![main]
