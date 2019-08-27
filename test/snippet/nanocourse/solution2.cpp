#include <iostream>
#include <vector>

struct Bitvector
{
    std::vector<uint64_t> data;

    Bitvector(size_t const count) : data((count + 63) / 64, 0u) {};

    bool read(size_t const i) const
    {
        return (data[i / 64] >> (63 - (i % 64))) & 1;
    }
};

int main()
{
    Bitvector B(10);

    std::cout << B.read(2) << std::endl; // prints 0
}
