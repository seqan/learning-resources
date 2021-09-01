#include <iostream>
#include <numeric>
#include <seqan3/std/ranges>
#include <vector>

#ifdef __cpp_lib_concepts // C++20 compiler
template <typename value_t, typename init_t>
concept accumulable_with = requires (value_t && value, init_t && init)
{
    { std::move(init) + std::forward<value_t>(value)} -> std::same_as<init_t>;
};
#else // pre C++20 compiler
template <typename value_t, typename init_t>
concept bool accumulable_with = requires (value_t && value, init_t && init)
{
    { std::move(init) + std::forward<value_t>(value)} -> init_t;
};
#endif

template <std::ranges::input_range range_t>
// Too strong constraint: std::is_arithmetic_v<std::ranges::range_value_t<range_t>>
    requires accumulable_with<std::ranges::range_value_t<range_t>, int32_t>
auto sum(range_t && data)
{
    auto common_range = data | std::views::common;
    return std::accumulate(std::ranges::begin(common_range), std::ranges::end(common_range), int32_t{});
}

template <typename range_t>
int32_t sum_generic(range_t const & data)
{
    return std::accumulate(std::begin(data), std::end(data), 0);  // Use algorithms
}

int32_t sum_old(std::vector<int32_t> const & data)
{
    return std::accumulate(data.begin(), data.end(), 0);  // Use algorithms
}

struct foo
{
    int var;

    int32_t operator+(int32_t const & rhs) const
    {
        return var + rhs;
    }

    friend int32_t operator+(int32_t const & lhs, foo const & rhs)
    {
        return rhs + lhs;
    }

};

int main()
{
    std::vector int_data{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    std::vector foo_data{foo{0}, foo{1}, foo{2}, foo{3}};

    std::cout << "The sum of my foo vector is " << sum(foo_data) << "\n";
    std::cout << "The sum of my int vector is " << sum(int_data) << "\n";

    return EXIT_SUCCESS;
}
