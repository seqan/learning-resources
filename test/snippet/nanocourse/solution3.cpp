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

    void write(size_t const i, bool const value)
    {
        uint64_t mask = static_cast<uint64_t>(1) << (63 - (i % 64));

        if (value == true)
            data[i / 64] |= mask;
        else
            data[i / 64] &= ~mask;
    }
};

int main()
{
    Bitvector B(10);

    std::cout << B.read(63) << std::endl; // prints 0
    B.write(63, 1);                       // writes a 1 at position 63 in the bitvector
    std::cout << B.read(63) << std::endl; // prints 1
    B.write(63, 0);                       // writes a 0 at position 63 in the bitvector
    std::cout << B.read(63) << std::endl; // prints 0 again
}
