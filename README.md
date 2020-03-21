# Huffman Encoding

## Description
Huffman Encoding is a commonly used data compression algorithm used notably in gzip and jpegs. The algorithm broadly takes as input a raw file and converts each octal in the file into a prefix-free code where octals appearing more frequently in the file encoded with shorter bitstrings. The basic Huffman encoding algorithm requires including the built Huffman tree along with the encoding, which includes many internal nodes which are unnecessary for decoding. For this reason canonized Huffman encoding, which includes additional steps to allow for deterministic tree creation from a list of bit lengths for each octal reduce the amount of unnecessary information transmitted, thus increasing compression factor at the cost of encoding time.

## Usage
- `src` contains the base source code for the host and device.
- `scripts` contains the high-level scripts to generate designs, synthesize them, and generate an output table.


## Algorithm
The Huffman encoding algorithm is executed in 7 stages:
```
frequency_histogram <- input

huffman_encoding:
    filtered_frequency_histogram <- filter(frequency_histogram)
    sorted_frequency_histogram <- sort(filtered_frequency_histogram)
    huffman_tree <- create_tree(sorted_frequency_list)
    length_histogram <- compute_bit_lengths(huffman_tree)
    truncated_length_histogram <- truncate_tree(length_histogram)
    canonized_length_histogram <- canonize_tree(truncated_length_histogram)
    encoding <- create_codeword(canonized_length_histogram)

output <- (encoding, canonized_length_histogram)
```

- Input: The algorithm assumes that the input file has already been processed and the frequency of each octal has been recorded in a histogram.
- `filter`: All octals that do not exist in the input file are removed from the histogram and are removed from consideration.
- `sort`: The frequency histogram is sorted in increasing order by frequency which is required for tree creation.
- `create_tree`: The sorted histogram is processed to create a huffman tree. Here the tree is represented as three arrays: left, right, and parent, with each value in the array representing an edge between two nodes.
- `compute_bit_length`: In order to canonize the tree the depth of each node in the tree (which translates to its bit length) is calculated and stored in a histogram.
- `truncate_tree`: The tree is rebalanced to prevent any codes from having a bit length greater than a preset value. In our algorithm the value is 27.
- `canonize_tree`: After rebalancing bit lengths of each octal is again computed.
- `create_codeword`: The codeword is generated for each octal following two rules: longer length codes have a higher numerical value than lower length codes and all codes with the same length have sequential numerical values. This allows for reconstructing the tree from just the bit lengths of each octal.

## Knobs
Due to the complex nature of the algorithm which includes many loops and internal arrays, the number of potential knobs is quite large and would produce a design space too large to synthesize in a reasonable time. For this reason we create two different design spaces in order to reduce the potential design space.

The knobs we utilized in both designs utilize loop unrolling of various loops and array partitioning of arrays accessed in the loop. Due to the limitations on design space size all arrays accessed within a loop are partitioned together, with the copy loops taking higher priority as these knobs have more granularity. 

### Design 1
This design is formed around identifying key loops where unrolling and array partitioning would be expected to achieve a linear increase in speedup at the cost of some area. Beyond those obvious loops knobs were created to maximize coverage of potential loops, with a slight focus on loops with higher total latency.

- `copy0`: Unroll factor and array partitioning factor for all loops that initialize all values of a array to 0.
- `copy1`: Unroll factor and array partitioning factor for all loops that copy all values of one array into another.
- `computehistogram`: Unroll factor of the compute_histogram loop in sorting.
- `computehistogram_partition`: Array partitioning factor of the compute_histogram loop in sorting.
- `resort`: Unroll factor and array partitioning factor for the re_sort loop in sorting.
- `processsymbols`: Unroll factor and array partitioning factor for the process_symbol loop in canonize_tree.
- `movenodes`: Unroll factor and array partitioning factor for the move_nodes loop in truncate_tree. 
- `assigncodeword`: Unroll factor and array partitioning factor for the assign_codeword loop in create_codeword.

We expect that due to their their simplicity, increasing `copy0` and `copy1` would linearly increase the performance for the loop provided that the array was partitioned to match. For this reason we kept the value of the unroll and partition factor the same for each design. `compute_histogram` is another loop that, because it accesses its arrays sequentially, would also benefit linearly from loop unrolling and partitioning. The unroll factor and array partition are separate due to the additional complexity of the loop making it unclear whether the array partition factor should be equal to the unroll factor. For `resort`, `processsymbols`, `movenodes`, and `assigncodeword`, it is unclear whether unrolling and partitioning arrays would improve performance, as they all have non-constant and non-sequential array access. These loops were chosen as they were the loops that made up the largest portion of the runtime of the algorithm. It would be helpful to separate unroll factor and partition factor as separate knobs, but with this many loops in consideration the size of the design space would be too large to run in a reasonable timeframe.

### Design 2
This design is a modification on the initial design and attempts to improve on some of the weaknesses of the initial design. The number of knobs is reduced and even more focus is placed on the longest running loops in an attempt to generate output points with a wider range. 

- `copy0`: Unroll factor and cyclic array partitioning factor for all loops that initialize all values of a array to 0.
- `copy1`: Unroll factor and cyclic array partitioning factor for all loops that copy all values of one array into another.
- `processsymbols`: Unroll factor for the process_symbol look in canonize_tree.
- `createtree`: Unroll factor for the create_tree_main loop in create_tree.
- `createtree_partition`: Array partition factor for the create_tree_main loop in create_tree.
- `assigncodeword`: Unroll factor for the assign_codeword loop in create_codeword.
- `assigncodeword_partition`: Array partition for the assign_codeword loop in create_codeword.

Though these knobs look similar to those of the initial design space, there are several key differences between the two. We again expect to see a linear speedup for increasing values of `copy0` and `copy1`, but in this design space have increased the step size in an attempt to further show the differences between designs with different values. The remaining five knobs focus on the three longest-running loops in the algorithm and each have a separate knob for unroll factor and array partition in order to gain further insight into the factors that cause designs to differ. `processsymbols` notably does not have a corresponding partition knob, simply due to the fact that all arrays accessed in the loop are also partitioned in `copy1`.
