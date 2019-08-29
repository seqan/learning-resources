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

//![count_table_computation]
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
//![count_table_computation]

//![main]
int main()
{
    std::string text{"mississippi"};
    std::string bwt = bwt_construction(text);

    // count table
    // -------------------------------------------------------------------------
    std::vector<uint16_t> count_table = compute_count_table(bwt);

    std::cout << "$ i m p s\n" << "---------\n";
    for (auto c : count_table)
        std::cout << c << " ";
    std::cout << '\n';
}
//![main]
