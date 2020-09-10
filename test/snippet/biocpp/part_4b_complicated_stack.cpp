#include <list>
#include <seqan3/std/algorithm>
#include <seqan3/std/ranges>
#include <vector>

int main()
{
    // std::list<int> list{1,2,3,4,5}; // does not compile
    std::vector<int> list{1,2,3,4,5};

    // std::sort(list.begin(), list.end()); // before C++20
    // std::ranges::sort(list); // since C++20
}
