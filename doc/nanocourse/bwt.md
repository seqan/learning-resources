# Compact Data Structures - FM-Index {#fm_index}

[TOC]

This HowTo introduces the Burrows-Wheeler transform and a well-known application thereof: the FM-Index.

\tutorial_head{Easy, 120 min, \ref bitvectors, [Compact Data Structures (Navarro)]
(https://www.cambridge.org/core/books/compact-data-structures/68A5983E6F1176181291E235D0B7EB44)}

# Motivation

In 1994, Burrows and Wheeler proposed a lossless compression algorithm (now used in bzip2). The algorithm transforms
a given text into the so called Burrows-Wheeler transform (BWT), a permutation of the text that can be reversed.
In general, the transformed text can be compressed better than the original text as in the BWT equal characters tend
to form consecutive runs which can be compressed using run-length encoders.

We will see that it is possible to conduct exact searches using only the BWT and some auxiliary tables.

# Theoretical Background: Burrows-Wheeler transform and LF-Mapping

The Burrows-Wheeler transform (BWT) can be obtained by the following steps:

1. Form a conceptual matrix \f$M\f$ whose rows are the \f$n\f$ cyclic shifts of the text \f$T\f$.
2. Lexicographically sort the rows of \f$M\f$.
3. Construct the transformed text \f$T^{bwt}\f$ by taking the last column \f$L\f$ of \f$M\f$.

\todo Picture.

Instead of actually constructing the huge matrix, we can also construct the suffix array in \f$O(n)\f$ time and
space, and then construct the BWT from that:

\f$
T^{bwt}[i]=\begin{cases}
~T[A[i]-1] & \text{ if } A[i]>0 \\
~\$ & \text{ else }
\end{cases}
\f$

One interesting property of the Burrows-Wheeler transform \f$T^{bwt}\f$ is that the original text \f$T\f$ can be
reconstructed by a reverse transform without any extra information. Therefor, we need the following definition:

**Definition (L-to-F mapping)**: Let \f$M\f$ be the sorted matrix of cyclic shifts of the text \f$T\f$. \f$LF\f$ is
a function \f$LF : [0..n) \rightarrow [0..n)\f$ that maps the rank of a cyclic shift \f$X\f$ to the rank of
\f$X^{(n)}\f$, i.e \f$X\f$ shifted by one to the right:

\f$LF(l) = f \Leftrightarrow M_f = M_l^{(n)}\f$

This property is the key when searching in the BWT.

In order to conduct the LF-Mapping efficiently, we need two helper data structures, namely the occurence table `Occ`
and the count table `C`.

# Helper data structures: Occ & C

Let \f$C : \Sigma \rightarrow [0..n]\f$ be a function that maps a character \f$c \in \Sigma\f$ to the total number
of occurrences in \f$T\f$ of the characters which are alphabetically smaller than \f$c\f$.

\assignment{Implement the count table}

Use this code to construct a BWT from a given input text:

\snippet test/snippet/nanocourse/bwt_count_table.cpp bwt_construction

For simplicity, we will only work on the alphabet \f$\Sigma = {\$, i, m, p, s}\f$.

Implement a function `std::vector<uint16_t> compute_count_table(std::string const & bwt)` that returns a count table
of length 5 with the counts for the characters `$, i, m, p, s` in that order.

Since we will often need the index of a given alphabet character, we will use the following helper function to map a
character to an integer:

\snippet test/snippet/nanocourse/bwt_count_table.cpp to_index

Given the following main function,

\snippet test/snippet/nanocourse/bwt_count_table.cpp main

Your program should output the following:

```
$ i m p s
---------
0 1 5 6 8
```

If you are inexperienced with C++, you can use the following backbone:

\hint
```cpp
std::vector<uint16_t> compute_count_table(std::string const & bwt)
{
    std::vector<uint16_t> count_table(5); // the prefix count table.

    for (auto chr : bwt)
    {
        // which positions in count_table need to be increased?
        for (size_t i = /*TODO: start position to be increased*/; i < /*TODO: end position to be increased*/; ++i)
            ++count_table[i]; // increase position i by 1.
    }

    return count_table;
}
```
\endhint

\endassignment

\solution

\snippet test/snippet/nanocourse/bwt_count_table.cpp count_table_computation

\endsolution

Let \f$Occ : \Sigma \times [0..n] \rightarrow [0..n]\f$ be a function that maps \f$(c,k) \in \Sigma \times [0..n]\f$
to the number of occurrences of \f$c\f$ in the prefix \f$L[0..k)\f$ of the transformed text \f$L\f$.

If we would implement the occurrence table the naive way, i.e. via five arrays of length \f$n\f$ of 64 bit integers
for each character, we would need \f$ 5 \cdot 64 \cdot n\f$ bits to store this array.
In order to reduce the space consumption, we can introduce a more compact data structure:

We will use five Bitvectors of length \f$n\f$, again one for each character,
where we set a \f$1\f$ at position \f$i\f$ iff the corresponding character appears at position \f$i\f$ in \f$L\f$.
That way we only use \f$5 \cdot \frac{n}{64} \f$ bits of space if we use the bitvector implementation
from the \ref bitvectors "bitvector tutorial".

\assignment{Implement the occurrence table via bitvectors}

We will use the bitvector data structure from the previous \ref bitvectors "bitvector tutorial".
For simplicity, copy and paste the following code summary into a file called `bitvector.hpp` in your source directory.
We will then be able to include the code into our current solution `cpp` file without any effort.

Since the occurrence table is a bit more complex, we will implement the occurrence functionality within a new struct.

```cpp
#include "bitvector.hpp"

struct occurrence_table
{
    // The list of bitvectors:
    std::vector<Bitvector> data;

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

If you are inexperienced with C++, you can use the following backbone:

\hint
```cpp
#include "bitvector.hpp"

struct occurrence_table
{
    // The list of bitvectors:
    std::vector<Bitvector> data;

    // We want that 5 bitvectors are filled depending on the bwt,
    // so let's customise the constructor of occurrence_table:
    occurrence_table(std::string const & bwt)
    {
        // resize the 5 bitvectors to the length of the bwt:
        data.resize(/*TODO: How many Bitvectors?*/, Bitvector(/*TODO: How large should each bitvector be?*/));

        // fill the bitvectors
        for (size_t i = 0; i < bwt.size(); ++i)
            data[to_index(/* TODO: get the i'th character in the bwt.*/)].write(i, 1);
    }
};
```
\endhint

\endassignment

\solution

\snippet test/snippet/nanocourse/bwt_occurrence_table.cpp occurrence_table_computation

\endsolution

Now when we use the occurrence table, we actually want to answer for example the query
\"How many characters \c 's' have I found up until position \c i in the bwt?\".

This means we have to count number of `1`'s the bitvector for character \c 's' up until position `i`.
Does this sound familiar? Indeed, it is a **rank query**.
We have implemented rank support for our bitvector data structure in the
\ref bitvectors "bitvector tutorial" so lets make use of this to implement access to the occurrence table.

\assignment{Access the occurrence table}

1. Adapt your constructor to also construct the helper data structures of your bitvector.
Choose a block size of `3` bits and a superblock size of `6` bits
(This is not recommended for real world data but serves as a toy example for the small bwt).
2. Implement the member function `size_t read(char chr, size_t i) const` that returns the number of
occurrences of `chr` up until position `i` in the bwt using the rank support for the bitvectors.

Now given the following main function we can print the actual occurrence table:

\snippet test/snippet/nanocourse/bwt_occurrence_table2.cpp main

The output should look like this:
```
   i p s s m $ p i s s i i
   -----------------------
$  0 0 0 0 0 1 1 1 1 1 1 1
i  1 1 1 1 1 1 1 2 2 2 3 4
m  0 0 0 0 1 1 1 1 1 1 1 1
p  0 1 1 1 1 1 2 2 2 2 2 2
s  0 0 1 2 2 2 2 2 3 4 4 4
```

If you are inexperienced with C++, you can use the following backbone:

\hint

```cpp
struct occurrence_table
{
    // The list of bitvectors:
    std::vector<Bitvector> data;

    // We want that 5 bitvectors are filled depending on the bwt,
    // so let's customise the constructor of occurrence_table:
    occurrence_table(std::string const & bwt)
    {
        // resize the 5 bitvectors to the length of the bwt:
        data.resize(5, Bitvector((bwt.size() + 63)/ 64));

        // fill the bitvectors
        for (size_t i = 0; i < bwt.size(); ++i)
            data[to_index(bwt[i])].write(i, 1);

        // construct the helper data structures
        for (Bitvector & bitv : data)
            /*TODO: Call the construct member function with block size 3 and superblock size 6*/;
    }

    bool read(char const chr, size_t const i) const
    {
        return /*TODO: For the bitvector corresponding to chr in data, call the rank member function at position i + 1*/;
    }
};
```
\endhint

\endassignment

\solution

\snippet test/snippet/nanocourse/bwt_occurrence_table2.cpp occurrence_table_computation

\endsolution

# Backward Search

Now that we have the helper data structures, we can conduct an efficient pattern search within our bwt.

The pseudo code to compute the number of occurrences of a pattern \f$P[0..m)\f$ in \f$T[0..n)\f$ is the following:

```python
(1)  count(P[0..m-1])
(2)  i = m - 1, a = 0, b = n - 1;
(3)  while ((a <= b) AND (i >= 0)) do
(4)      c = P[i];
(5)      a = C(c) + Occ(c, a - 1)
(6)      b = C(c) + Occ(c, b) - 1;
(7)      i = i - 1;
(8)
(9)  if (b < a) then return not found;
(10) else return found (b - a + 1) occurrences;
```

\assignment{Backward search}

Using all of your code so far, implement a function
```cpp
size_t count(std::string const & P,
             std::string const & bwt,
             std::vector<uint16_t> const & C,
             occurrence_table const & Occ)
{
    // your code goes here
}
```
that returns the number of occurrences of pattern `P`.

Given the following main function:

\snippet test/snippet/nanocourse/backward_search.cpp main

Your program should output

```
2
1
```
If you are inexperienced with C++, you can use the following backbone:

\hint

```cpp
size_t count(std::string const & P,
             std::string const & bwt,
             std::vector<uint16_t> const & C,
             occurrence_table const & Occ)
{
    int64_t i = /*TODO: where do we start our search in the pattern?*/;
    size_t a =  /*TODO: what is the lower bound to start?*/;
    size_t b =  /*TODO: what is the upper bound to start?*/;

    while ((a <= b) && (i >= 0))
    {
        char c = /*TODO: The character at position i in P.*/;

        // Note that () ? x : y in the next line takes care of the case that a is 0
        // because a - 1 would be negative and Occ.read at a negative position is invalid.
        a = /*TODO: C at the index of c*/ + (a ? Occ.read(c, a - 1) : 0);

        b = /*TODO: C at the index of c*/ +  /*TODO: read Occ at position b with character c*/ - 1;

        i = i - 1;
    }

    if (b < a)
        return 0;
    else
        return (b - a + 1);
}
```
\endhint

\endassignment

\solution

\snippet test/snippet/nanocourse/backward_search.cpp solution

\endsolution

Here is the full solution in case you need it:

\solution

\include test/snippet/nanocourse/backward_search.cpp

\endsolution

# Real data example with SeqAn3

In the following section, we will conduct a search in an FM-index using SeqAn3.
We will search for 1'000'000 reads within chr4 of the Drosophila genome and count how many reads occur more than 100
times, indicating a repeat region!

As a first step, set up SeqAn3 by following the [Setup tutorial](https://docs.seqan.de/seqan/3-master-user/setup.html)
and add an executable `count_occurrences` to the `CMakeLists.txt`.
Create a new file `count_occurrences.cpp` in the `source` directory with the following content:

```cpp
#include <seqan3/io/sequence_file/all.hpp>
#include <seqan3/search/all.hpp>

using namespace seqan3;

int main()
{
    sequence_file_input reference_in{"../reference.fasta.gz"}; // Read the reference (only 1 contig).
    sequence_file_input query_in{"../reads.fasta.gz", fields<field::SEQ>{}}; // Read the query file (multiple contigs).

    fm_index index{get<field::SEQ>(*reference_in.begin())}; // Create an FM-index from the reference.

    size_t counter{0u};

    for (auto & [seq] : query_in) // For each read in our query file.
        counter += std::ranges::size(search(seq, index)) > 100; // Count if there are more than 100 occurrences.

    std::cout << "Found " << counter << " queries with more than 100 exact hits.\n";
}
```

The above code basically does the same things as you just implemented in this nanocourse.
The SeqAn library provides a lot of commonly needed functionality when working with high throughput sequencing,
such that you do not need to implement your own data structures and algorithms.

We use the [`sequence_file_input`](https://docs.seqan.de/seqan/3-master-user/classseqan3_1_1sequence__file__input.html)
to handle reference and read input - the format and content of the files is taken care of automatically.
The [`fm_index`](https://docs.seqan.de/seqan/3-master-user/classseqan3_1_1fm__index.html) encapsulates a suffix array, occurrence table and count table just like we implemented it ourselves before.
By calling [`search`](https://docs.seqan.de/seqan/3-master-user/group__submodule__search__algorithm.html) we can
compute every position that a query matches the index; unless configured in another way, this will report *every exact*
hit within our reference.

To use this example, download the
[reference file](http://ftp.imp.fu-berlin.de/pub/SeqAn/learning_resources/nanocourse/reference.fasta.gz) and
[read file](http://ftp.imp.fu-berlin.de/pub/SeqAn/learning_resources/nanocourse/reads.fasta.gz) into the `tutorial`
directory.
Run `cmake -DCMAKE_CXX_FLAGS="-O3 -march=native" ../source` within the `build` directory and execute the program via
`./count_occurrences` and measure runtime and memory consumption via, for example,
`/usr/bin/time -v ./count_occurrences`.

If you want to learn more about SeqAn3, we recommend you start with the [tutorials]
(https://docs.seqan.de/seqan/3-master-user/tutorial_first_example.html).
