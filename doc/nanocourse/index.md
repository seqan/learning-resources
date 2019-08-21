# Compact Data Structures - An introduction {#compact_data_structures}

[TOC]

This HowTo gives an introduction into compact data structures. It is designed independently of SeqAn but
it covers design patterns and concepts that are the foundations of efficient C++ used throughout the library.

\tutorial_head{Easy, 120 min, , [Compact Data Structures (Navarro)](https://www.cambridge.org/core/books/compact-data-structures/68A5983E6F1176181291E235D0B7EB44)}

# Motivation

A compact data structure maintains data, and the desired extra data structures
over it, in a form that not only uses less space than usual, but is able to access and
query the data in compact form, that is, without decompressing them. Thus, a com-
pact data structure allows us to fit and efficiently query, navigate, and manipulate
much larger datasets in main memory than what would be possible if we used the
data represented in plain form and classical data structures on top.

A good source of reading (which is online available for free):
[Gonzalo Navarro, Compact Data Structures]
(https://www.cambridge.org/core/books/compact-data-structures/68A5983E6F1176181291E235D0B7EB44)

# Bitvectors

Bitvectors are fundamental in the implementation of most compact data structures.
Therefore, an efficient implementation is of utmost importance.

A bitvector is a bit array \f$B[1, n]\f$ of length \f$n\f$ of values \f$v \in {0,1}\f$ (bits).

In C++ there is no type that defines a single bit, such that you could use `std::vector<bit>` for this purpose.
If you would use, let's say, `std::vector<bool>` instead, you would use 1 byte = 8 bit for each entry,
or even worse `std::vector<int>` which uses 4 byte = 32 bit.

Let us design a compact data structure that stores single bits as values
while still supporting the basic operations:

* \f$read(B, i)\f$: returns the bit \f$B[i]\f$, for any \f$1 \leq i \leq n\f$.
* \f$write(B, i, x)\f$: writes the bit \f$x \in {0,1}\f$ to \f$B[i]\f$, for any \f$1 \leq i \leq n\f$.

## A compact representation of bitvectors

As noted above we need to represent bits using larger entities available in the C++ language.
It usually pays off if you choose an integer with the size of a machine word `w`, which is 64 on modern architectures.
We will use a `std::vector` over `uint64_t` for our representation.

\assignment{A compact bitvector}

Construct a `std::vector` over `uint64_t` of a length that can store at least 600 bits, initialised with zeros.

\endassignment

\solution

\include test/snippet/nanocourse/solution1.cpp

\endsolution

### Access the compact bitvector

We now want to support access to our bitvector by implementing the function `read(B, i)`.

--- explain bit operations ---

\assignment{Reading}

Complete the following implementation of a function that can access the compact bitvector:

```cpp

bool read(std::vector<uint64_t> const & B, size_t const i)
{
    // ... your implementation goes here

    // return x; // x = true if the bit is set to 1 and x = false otherwise
}

```

\endassignment

\solution

\include test/snippet/nanocourse/solution2.cpp

\endsolution

### Write to the compact bitvector

We now want to support writing to our bitvector by implementing the function `write(B, i, x)`.
This is a bit more tricky.
Hints?

\assignment{Writing}

Complete the following implementation of a function that can access the compact bitvector:

```cpp

void write(std::vector<uint64_t> & B, size_t const i, bool const x)
{
    // ... your implementation goes here
}

```

\endassignment

\solution

\include test/snippet/nanocourse/solution3.cpp

\endsolution

## Rank and select

Two very useful operations that a bitvector should support are the following:

* \f$rank_v(B, i)\f$: returns the number of occurrences of bit \f$v \in {0, 1}\f$ in \f$B[1, i]\f$,
for any \f$0 \leq i \leq n\f$; in particular \f$rank_v(B, 0) = 0\f$. If omitted, we assume \f$v = 1\f$.
* \f$select_v(B, j)\f$: returns the position in \f$B\f$ of the j-th occurrence of bit \f$v \in {0, 1}\f$,
for any \f$j \geq 0\f$; we assume \f$select_v(B, 0) = 0\f$ and \f$select_v(B, j) = n + 1\f$ if \f$j >
rank_v(B, n)\f$. If omitted, we assume \f$v = 1\f$.

We will implement those operation for our compact bitvector representation.

### Helper data structures

In order to support rank and select queries we need two helper data structures (given machine world length `w`):

1. **Superblocks**: We divide the bitvector into superblocks of length `s = w · k bits`,
   where `k` is parameter to choose.
   We then store the store the rank values at the beginnings of the corresponding superblock in an array `R`.

2. **Blocks**: We further divide the bitvector into blocks of length `w`.
   We then store the store the rank values at the beginnings of the corresponding block,
   **but relatively to their corresponding superblock**, in an array `R'`.

--- picture ---

\assignment{Data types}

The block and superblock values obviously need to be stored using an arithmetic data type
(e.g. `uint8_t`, `uint16_t`, `uint32_t`, `uint64_t`).
Given arbitrary bitvector of length n, word length `w = 64 bits` and a superblock length of `s = 64 * 25 = 1600 bits`.
Which type would you choose for superblock entries, and which type for block entries and why?

\endassignment

\solution

If a superblock spans is 1600 bits, then the last block value within a superblock can at most count 1599 `1s`.
Thus, a `uint16_t` is the smallest type that is still able represent this number
and should be preferred to larger typed, to reduces the space of the block array.

Since we do not know how large our bitvector might be, one should choose a large data type, like `uint64_t`,
to store to prefix sums for the superblock values.

\endsolution

Now that we now which data types to work with, let's implement the data structures.

\assignment{Compute the block and superblock arrays}

Given a bitvector `B` from the previous assignments that can store `6400 bits` (what's the length?),
compute the entries of the arrays by iterating over the bitvector.

With the following main function:

```cpp
int main()
{
    std::vector<uint64_t> B(6400 / 64, 2);

    std::vector<uint16_t> blocks(6400 / 64, 0);
    std::vector<uint64_t> superblocks(6400 / 1600, 0);

    construct(blocks, superblocks, B);

    for (auto a : superblocks)
        std::cout << a << " ";
    std::cout << std::endl;
}
```

your program should print the following:

```
25 50 75 100
```

If you print your blocks too, there should be 100 entries: `1,2,...,25,1,2,...25,1,2,...,25,1,2,...25`.

If you are not so experienced with C++, you can use the following backbone:

\hint

```cpp
void construct(std::vector<uint16_t> & blocks,
               std::vector<uint64_t> & superblocks,
               std::vector<uint64_t> const & B)
{
    size_t block_pos{0u};
    size_t superblock_pos{0u};

    uint16_t block_count{0u};
    uint64_t superblock_count{0u};

    // Note that we start from 1 to make the modulo computations easier.
    for (size_t i = 1u; i <= B.size() * 64; ++i)
    {
        if (/*whenever we reach the end of a block. Hint: use the modulo operation on i*/)
        {
            // TODO: store value of count in the current block entry

            ++block_pos; // move to the next position

            if (/*whenever we even reach the end of a superblock. Hint: use the modulo operation on i*/)
            {
                superblock_count += block_count; // update superblock count

                // TODO: store the value of superblock_count into the current super block entry

                ++superblock_pos; // move to the next position
                block_count = 0u;   // reset block count
            }
        }

        if (/*bitvector B has a 1 at position i - 1*/)
            ++block_count;
    }
}
```
\endhint
\endassignment

\solution

\include test/snippet/nanocourse/solution4.cpp

Note that most compilers provide special bit operations on integers.
One example is **popcount** that counts the number of `1s` in an integer.
This would be very helpful in our application because instead of iterating over every position in our bitvector `B`
we could directly use popcount on every entry of b to get the value for the block
(of course this only works since we chose the block size wisely).

The code could then look like this (**This is compiler specific (GCC)**):

\include test/snippet/nanocourse/solution4_intrinsic.cpp

If you have some time to spare, increase the size of B, and do come runtime tests for construction.
The construction using popcount should be considerably faster.

\endsolution
