#include <iostream>
#include <numeric>
#include <vector>

int32_t sum(std::vector<int32_t> const & data)
{
    int32_t intermediate_sum{};
    for (int32_t element : data) // Calls in the background data.begin() and data.end().
        intermediate_sum += element;

    return intermediate_sum;
}

int32_t sum_short(std::vector<int32_t> const & data)
{
    return std::accumulate(data.begin(), data.end(), 0);  // Use algorithms
}

int main(int argc, char const * argv[])
{
    std::vector<int32_t> all_numbers{};
    all_numbers.reserve(argc - 1);

    for (int index = 1; index < argc; ++index)
        all_numbers.push_back(std::atoi(argv[index]));

    std::cout << "The sum of all numbers is " << sum(all_numbers) << "\n";

    return EXIT_SUCCESS;
}

/* Solution with ranges. Could be shown later as well.
int main(int argc, char const * argv[])
{
    auto all_numbers = std::views::counted(std::addressof(argv[1]), argc - 1)
                     | std::views::transform([] (auto && c_string) { return std::atoi(c_string); })
                     | seqan3::views::to<std::vector>;

    std::cout << "The sum of all numbers is " << sum(all_numbers) << "\n";

    return EXIT_SUCCESS;
}
*/
