#include <iostream>
#include <string>
#include <tuple>

#ifdef __cpp_lib_concepts // C++20 compiler
template <typename T>
concept arithmetic = std::is_arithmetic_v<T>;
#else // pre C++20 compiler
template <typename T>
concept bool arithmetic = std::is_arithmetic_v<T>;
#endif

static_assert(arithmetic<int>);
static_assert(arithmetic<float>);
static_assert(arithmetic<long>);
// static_assert(arithmetic<std::string>);

int add(int a, int b)
{
    return a + b;
}

unsigned add(unsigned a, unsigned b)
{
    return a + b;
}

float add(float a, float b)
{
    return a + b;
}

template <typename T>
T add_unconstrained(T a, T b) // duck typing
{
    // Every type that implements `operator+` is valid and compiles
    return a + b;
}

template <arithmetic T>
T add_concept(T a, T b)
{
    return a + b;
}

void add_strings()
{
    std::string a{"HELLO "};
    std::string b{"WORLD"};
    std::cout << add_unconstrained(a, b) << std::endl;
    // std::cout << add_concept(a, b) << std::endl; // WILL NOT COMPILE
}

void add_integrals()
{
    int a{2};
    int b{6};

    std::cout << add_unconstrained(a, b) << std::endl;
    std::cout << add_concept(a, b) << std::endl;
}

int main()
{
    add_strings();
    add_integrals();
    return 0;
}
