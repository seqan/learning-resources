#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

int32_t sum(std::vector<int32_t> const & data)
{
    return std::accumulate(data.begin(), data.end(), 0);  // Use algorithms
}

int main()
{
    std::vector<int32_t> int_data{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto average = [&] (double sum)
    {
        return sum / int_data.size();
    };

    std::cout << "The sum of my vector is " << sum(int_data) << "\n";
    std::cout << "The average: " << average(sum(int_data)) << "\n";

    return EXIT_SUCCESS;
}
