#include <iostream>
#include <vector>

void write(std::vector<uint64_t> & B, size_t const i, bool const x)
{
    uint64_t mask = static_cast<uint64_t>(1) << (63 - (i % 64));

    if (x == 1)
        B[i / 64] |= mask;
    else
        B[i / 64] &= ~mask;
}

bool read(std::vector<uint64_t> const & B, size_t const i)
{
    return (B[i / 64] >> (63 - (i % 64))) & 1;
}

int main()
{
    std::vector<uint64_t> B(10, 0u);

    std::cout << read(B, 63) << std::endl; // prints 0
    write(B, 63, 1);                       // writes a 1 at position 63 in the bitvector
    std::cout << read(B, 63) << std::endl; // prints 1
    write(B, 63, 1);                       // writes a 0 at position 63 in the bitvector
    std::cout << read(B, 63) << std::endl; // prints 0 again
}
