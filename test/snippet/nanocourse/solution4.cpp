#include <iostream>
#include <vector>

struct Bitvector
{
    std::vector<uint64_t> data;
    std::vector<uint16_t> blocks;
    std::vector<uint64_t> superblocks;

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

    size_t size() const
    {
        return data.size() * 64u;
    }

    void construct(size_t const block_size = 64u, size_t const superblock_size = 512u)
    {
        blocks = std::vector<uint16_t>((size() + block_size - 1) / block_size, 0u);
        superblocks = std::vector<uint64_t>((size() + superblock_size - 1) / superblock_size, 0u);

        size_t block_pos{0u};
        size_t super_block_pos{0u};

        uint16_t block_count{0u};
        uint64_t super_block_count{0u};

        for (size_t i = 0u; i < size(); ++i)
        {
            if (i % block_size == 0u)
            {
                if (i % superblock_size == 0u)
                {
                    super_block_count += block_count; // update superblock count

                    superblocks[super_block_pos] = super_block_count;

                    ++super_block_pos; // move to the next position
                    block_count = 0u;   // reset block count
                }

                blocks[block_pos] = block_count;

                ++block_pos; // move to the next position
            }

            if (read(i) == true)
                ++block_count;
        }
    }
};

int main()
{
    Bitvector B(6400);

    for (size_t i = 0; i < 100; ++i)
        B.write(i * 64, 1);

    B.construct(64, 1600);

    for (auto a : B.blocks)
        std::cout << a << " ";
    std::cout << std::endl;

    for (auto a : B.superblocks)
        std::cout << a << " ";
    std::cout << std::endl;
}
