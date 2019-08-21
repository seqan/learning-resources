#include <iostream>
#include <vector>

bool read(std::vector<uint64_t> const & B, size_t const i)
{
    return (B[i / 64] >> (63 - (i % 64))) & 1;
}

int main()
{
    std::vector<uint64_t> B(10, 0u);

    std::cout << read(B, 2) << std::endl; // prints 0
}
