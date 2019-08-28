#include <iostream>
#include <vector>

struct Bitvector
{
    std::vector<uint64_t> data;

    Bitvector(size_t const count)
    {
        data.resize((count + 63) / 64); // the +63 are a trick to round up the fraction.
    }

    bool read(size_t const i) const
    {
        return (data[i / 64] >> (63 - (i % 64))) & 1;
    }
};

//![main]
int main()
{
    Bitvector B(10);

    std::cout << B.read(2) << '\n'; // prints 0
}
//![main]
