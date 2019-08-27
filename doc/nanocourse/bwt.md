# Compact Data Structures - FM-Index {#fm_index}

[TOC]

TODO

\tutorial_head{Easy, 120 min, \ref bitvectors, [Compact Data Structures (Navarro)](https://www.cambridge.org/core/books/compact-data-structures/68A5983E6F1176181291E235D0B7EB44)}

# Motivation

Burrows and Wheeler proposed in 1994 a lossless compression algorithm (now
used in bzip2). The algorithm transforms a given text into the so called Burrows-
Wheeler transform (BWT), a permutation of the text that can be reversed.
The transformed text can in general be better compressed than the original text
as in the BWT equal characters tend to form consecutive runs which can be com-
pressed using run-length encoders.

We will see that it is possible to conduct exact searches using only the
BWT and some auxiliary tables.

# Theoretical Background: Burrows-Wheeler transform and LF-Mapping

The Burrows-Wheeler transform (BWT) can be obtained by the following steps:

1. Form a conceptual matrix \f$M\f$ whose rows are the \f$n\f$ cyclic shifts of the text \f$T\f$.
2. Lexicographically sort the rows of \f$M\f$.
3. Construct the transformed text \f$T^{bwt}\f$ by taking the last column \f$L\f$ of \f$M\f$.

\todo{picture}

Instead of actually constructing the huge matrix, we can also construct the suffix array in O(n) time and space,
and then construct the BWT from that:

\f$T^{bwt}[i] = todo \f$

One interesting property of the Burrows-Wheeler transform  \f$T^{bwt}\f$ is that the original
text \f$T\f$ can be reconstructed by a reverse transform without any extra information.
Therefore we need the following definition:

> **Definition (L-to-F mapping)**: Let \f$M\f$ be the sorted matrix of cyclic shifts of the
text \f$T\f$. \f$LF\f$ is a function \f$LF : [1..n] -> [1..n]\f$ that maps the rank of a cyclic shift \f$X\f$ to
the rank of \f$X^{(n)}\f$ which is \f$X\f$ shifted by one to the right:

\f$LF(l) = f <=> M_f = M_l^{(n)}\f$

This property is the key to when searching in the BWT.

In order to conduct the LF-Mapping efficiently, we need two helper data structures,
namely the occurence table `Occ` and the count table `C`.

# Helper data structures: Occ & C

Let \f$C : \sigma \rightarrow [0..n]\f$ be a function that maps a character \f$c \in \sigma\f$ to the total number
of occurrences in \f$T\f$ of the characters which are alphabetically smaller than \f$c\f$.

\assignment{Implement the count table}

Use this code to construct a BWT from a given input text:

\snippet test/snippet/nanocourse/bwt_count_table.cpp bwt_construction

For simplicity, we will only work on the alphabet \f$\sigma = {\$, i, m, p, s}\f$.

Implement a function `std::vector<uint16_t> compute_count_table(std::string const & bwt)` that returns a count table
of length 5 with the counts for the characters `$, i, m, p, s` in that order.

Given the following main function,

\snippet test/snippet/nanocourse/bwt_count_table.cpp main

Your program should output the following:

```
$ i m p s
---------
0 1 5 6 8
```

Here is a backbone for the function if you need it:

\hint
```cpp
std::vector<uint16_t> compute_count_table(std::string const & bwt)
{
    std::vector<uint16_t> count_table(5); // the prefix count table.

    for (auto chr : bwt)
    {
        switch (chr)
        {
            case '$':
                /*TODO: which positions in count_table need to be increased?*/;
                break;
            case 'i':
                /*TODO: which positions in count_table need to be increased?*/;
                break;
            case 'm':
                /*TODO: which positions in count_table need to be increased?*/;
                break;
            case 'p':
                /*TODO: which positions in count_table need to be increased?*/;
                break;
            default: break; // none have to be increased for the last character 's' (prefix sums)
        }
    }

    return count_table;
}
```
\endhint

\endassignment

\solution

\snippet test/snippet/nanocourse/bwt_count_table.cpp count_table_computation

\endsolution

Let \f$Occ : \sigma * [1.. n] \rightarrow [1..n]\f$ be a function that maps \f$(c,k) \in \sigma [1..n]\f$ to the number
of occurrences of \f$c\f$ in the prefix \f$L[1..k]\f$ of the transformed text \f$L\f$.

If we would implement the occurrence table the naive way, namely via five arrays of length n of 64 bit integers
for each character, we would need \f$ 5 \cdot 64 \cdot n\f$ bits for storing this array.
In order to reduce the space consumption, we can introduce a more compact data structure:

We will use five Bitvectors of length n, again one for each character,
where we set a \f$1\f$ at position \f$i\f$ iff the corresponding character appears at position \f$i\f$ in \f$L\f$.
That way we only use \f$5 \cdot \frac{n}{64} \f$ bits of space if we use the bitvector implementation
from the \ref bitvectors "bitvector tutorial".

\assignment{Implement the occurrence table via bitvectors}

Copy the code from the \ref bitvectors "bitvector tutorial" for accessing the bitvector data structure.

Since the occurrence table is a bit more complex, we implement the functionality within a new struct.

```cpp
struct occurrence_table
{
    // Lets's rename the type `std::vector<uint64_t>` to `bitvector` for a more readable code:
    using bitvector = std::vector<uint64_t>;

    // The list of bitvectors:
    std::vector<bitvector> bitvector_data;

    // We want that 5 bitvectors are filled depending on the bwt,
    // so let's customise the constructor of occurrence_table:
    occurrence_table(std::string const & bwt)
    {
        // your code
    }
};
```

Implement the function the constructor that initialises the five bitvectors for the characters `$, i, m, p, s`.
The bitvectors should have a \f$1\f$ set at position \f$i\f$ iff the corresponding character appears at
position \f$i\f$ in \f$L\f$ (the bwt).

Given the following main function:

\snippet test/snippet/nanocourse/bwt_occurrence_table.cpp main

Your output should look like this:
```
   i p s s m $ p i s s i i
   -----------------------
$  0 0 0 0 0 1 0 0 0 0 0 0
i  1 0 0 0 0 0 0 1 0 0 1 1
m  0 0 0 0 1 0 0 0 0 0 0 0
p  0 1 0 0 0 0 1 0 0 0 0 0
s  0 0 1 1 0 0 0 0 1 1 0 0
```

Here is a backbone for the function if you need it:

\hint
```cpp
void write(bitvector & B, size_t const i, bool const x)
{
    uint64_t mask = static_cast<uint64_t>(1) << (63 - (i % 64));

    if (x == 1)
        B[i / 64] |= mask;
    else
        B[i / 64] &= ~mask;
}

bool read(bitvector const & B, size_t const i)
{
    return (B[i / 64] >> (63 - (i % 64))) & 1;
}

struct occurrence_table
{
    // Lets's rename the type `std::vector<uint64_t>` to `bitvector` for a more readable code:
    using bitvector = std::vector<uint64_t>;

    // The list of bitvectors:
    std::vector<bitvector> bitvector_data;

    // We want that 5 bitvectors are filled depending on the bwt,
    // so let's customise the constructor of occurrence_table:
    occurrence_table(std::string const & bwt)
    {
        // resize the 5 bitvectors to the length of the bwt:
        bitvector_data = std::vector<bitvector>(5, bitvector(bwt.size()));

        // fill the bitvectors
        for (size_t i = 0; i < bwt.size(); ++i)
        {
            switch (bwt[i])
            {
                case '$':
                    /* TODO: set the first bitvector at position i to 1.*/;
                    break;
                case 'i':
                    /* TODO: set the second bitvector at position i to 1.*/;
                    break;
                case 'm':
                    /* TODO: set the third bitvector at position i to 1.*/;
                    break;
                case 'p':
                    /* TODO: set the fourth bitvector at position i to 1.*/;
                    break;
                case 's':
                    /* TODO: set the fifth bitvector at position i to 1.*/;
                    break;
                default: break;
            }
        }
    }
};
```
\endhint

\endassignment

\solution

\snippet test/snippet/nanocourse/bwt_occurrence_table.cpp occurrence_table_computation

\endsolution

Now when we use the occurrence table, we want to answer for example the query
"How many characters `s` have I found up until position `i` in the bwt"?

This means we have to count number of `1`'s the bitvector for character `s` up until position `i`.
Does this sound familiar? Yes, it is a **rank query**.
We have implemented rank and select support for our bitvector data structure in the
\ref bitvectors "bitvector tutorial" so lets make use of this to implement access to the occurrence table.

\assignment{Access the occurrence table}

1. Copy and paste the rank support from the \ref bitvectors "bitvector tutorial".
2. Add the following two names and two lists to your `occurence_table` struct:

```cpp
    using block = std::vector<uint16_t>;
    using superblock = std::vector<uint64_t>;

    // The list of blocks:
    std::vector<block> block_data;
    // The list of superblocks:
    std::vector<superblock> superblock_data;
```

3. Adapt your constructor to also compute and store the blocks and superblocks for each bitvector.
4. Implement function `size_t read(occurence_table const & Occ, char chr, size_t i)` that returns number of
occurrence of `chr` up until position `i` in the bwt using the rank support for the bitvectors.

\endassignment

# Backward Search

count

