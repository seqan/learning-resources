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

        for (size_t i = 0; i < number_of_bits; ++i)
        {
            if (i % block_size == 0)
            {
                if (i % superblock_size == 0)
                {
                    super_block_count += block_count; // update superblock count

                    superblocks[super_block_pos] = super_block_count;

                    ++super_block_pos; // move to the next position
                    block_count = 0;   // reset block count
                }

                blocks[block_pos] = block_count;

                ++block_pos; // move to the next position
            }

            if (read(i) == true)
                ++block_count;
        }
    }

    //![solution]
    uint64_t rank(size_t const i) const
    {
        uint64_t rank{0};

        rank += superblocks[(i - 1) / superblock_size];
        rank += blocks[(i - 1) / block_size];

        for (size_t j = ((i - 1) / block_size) * block_size; j < i; ++j)
        {
            rank += read(j);
        }
        return rank;
    }
    //![solution]
};

//![main]
int main()
{
    Bitvector B(6400);

    for (size_t i = 0; i < 100; ++i)
        B.write(i * 64, 1);

    B.construct(64, 1600);

    std::cout << B.rank(1) << '\n';  // 1
    std::cout << B.rank(64) << '\n'; // 1
    std::cout << B.rank(65) << '\n'; // 2
}
//![main]
