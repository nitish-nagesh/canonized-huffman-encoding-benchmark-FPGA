#include "huffman.h"
#include "assert.h"
void truncate_tree(
    /* input */ ac_int<SYMBOL_BITS, false> input_length_histogram[TREE_DEPTH],
    /* output */ ac_int<SYMBOL_BITS, false> output_length_histogram1[TREE_DEPTH],
    /* output */ ac_int<SYMBOL_BITS, false> output_length_histogram2[TREE_DEPTH]
) {
    // Copy into temporary storage to maintain dataflow properties
    copy_input: for(int i = 0; i < TREE_DEPTH; i++) {

        output_length_histogram1[i] = input_length_histogram[i];
    }

    ac_int<SYMBOL_BITS, false> j = MAX_CODEWORD_LENGTH;
    move_nodes: for(int i = TREE_DEPTH - 1; i > MAX_CODEWORD_LENGTH; i--) {
        // Look to see if there is any nodes at lengths greater than target depth
        reorder: while(output_length_histogram1[i] != 0) {
            if (j == MAX_CODEWORD_LENGTH) {
                // Find deepest leaf with codeword length < target depth
                do {
                    j--;
                } while(output_length_histogram1[j] == 0);
            }

            // Move leaf with depth i to depth j+1.
            output_length_histogram1[j] -= 1; // The node at level j is no longer a leaf.
            output_length_histogram1[j+1] += 2; // Two new leaf nodes are attached at level j+1.
            output_length_histogram1[i-1] += 1; // The leaf node at level i+1 gets attached here.
            output_length_histogram1[i] -= 2; // Two leaf nodes have been lost from level i.

            // now deepest leaf with codeword length < target length
            // is at level (j+1) unless j+1 == target length
            j++;
        }
    }

    // Copy the output to meet dataflow requirements and check the validity
    int limit = 1;
 copy_output: for(int i = 0; i < TREE_DEPTH; i++) {
        output_length_histogram2[i] = output_length_histogram1[i];
        // assert(output_length_histogram1[i] >= 0);
        // assert(output_length_histogram1[i] <= limit);
        limit *= 2;
    }
}
