#include <cinttypes>
#include <vector>

struct Bitvector
{
    std::vector<uint64_t> data;

    Bitvector(size_t const count)
    {
        data.resize((count + 63) / 64); // the +63 is a trick to round up the fraction.
    }
};

int main()
{
    Bitvector B(10);
}
