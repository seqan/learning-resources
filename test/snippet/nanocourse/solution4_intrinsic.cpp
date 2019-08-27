#include <iostream>
#include <vector>

struct Bitvector
{
    std::vector<uint64_t> data;
    std::vector<uint16_t> blocks;
    std::vector<uint64_t> superblocks;

    uint64_t block_size;
    uint64_t superblock_size;

    Bitvector(size_t const count)
    {
        data.resize((count + 63) / 64); // the +63 are a trick to round up the fraction.
    }

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

    void construct(size_t const new_block_size = 64, size_t const new_superblock_size = 512)
    {
        block_size = new_block_size;
        superblock_size = new_superblock_size;

        size_t number_of_bits = data.size() * 64;

        blocks.resize((number_of_bits + block_size - 1) / block_size, 0);
        superblocks.resize((number_of_bits + superblock_size - 1) / superblock_size, 0);

        size_t block_pos{0};
        size_t super_block_pos{0};

        uint16_t block_count{0};
        uint64_t super_block_count{0};

        // now we only need to iterate over each 64bit integer because we can count the 1's en bloc
        for (size_t i = 0; i < data.size(); ++i)
        {
            if ((i * 64) % block_size == 0)
            {
                if ((i * 64) % superblock_size == 0)
                {
                    super_block_count += block_count; // update superblock count

                    superblocks[super_block_pos] = super_block_count;

                    ++super_block_pos; // move to the next position
                    block_count = 0;   // reset block count
                }

                blocks[block_pos] = block_count;

                ++block_pos; // move to the next position
            }

            block_count += __builtin_popcountll(data[i]);
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
    std::cout << '\n';

    for (auto a : B.superblocks)
        std::cout << a << " ";
    std::cout << '\n';
}
