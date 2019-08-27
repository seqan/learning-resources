#include <cinttypes>
#include <vector>

struct Bitvector
{
    std::vector<uint64_t> data;

    Bitvector(size_t const count) : data((count + 63) / 64, 0u) {};
};

int main()
{
    Bitvector B(10);
}
