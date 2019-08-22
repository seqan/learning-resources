#include <iostream>
#include <vector>

bool read(std::vector<uint64_t> const & B, size_t const i)
{
    return (B[i / 64] >> (63 - (i % 64))) & 1;
}

void construct(std::vector<uint16_t> & blocks,
               std::vector<uint64_t> & superblocks,
               std::vector<uint64_t> const & B)
{
    size_t block_pos{0u};
    size_t super_block_pos{0u};

    uint16_t block_count{0u};
    uint64_t super_block_count{0u};

    for (size_t i = 0u; i < B.size() * 64; ++i)
    {
        if (i % 64 == 0u)
        {
            if (i % 1600 == 0u)
            {
                super_block_count += block_count; // update superblock count

                superblocks[super_block_pos] = super_block_count;

                ++super_block_pos; // move to the next position
                block_count = 0u;   // reset block count
            }

            blocks[block_pos] = block_count;

            ++block_pos; // move to the next position
        }

        if (read(B, i) == true)
            ++block_count;
    }
}

int main()
{
    std::vector<uint64_t> B(6400 / 64, 2);

    std::vector<uint16_t> blocks(6400 / 64, 0);
    std::vector<uint64_t> superblocks(6400 / 1600, 0);

    construct(blocks, superblocks, B);

    for (auto a : blocks)
        std::cout << a << " ";
    std::cout << std::endl;
}
