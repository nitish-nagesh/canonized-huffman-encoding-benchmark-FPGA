#include "huffman.h"
#include "assert.h"
void compute_bit_length (
    /* input */ ac_int<SYMBOL_BITS, false> parent[INPUT_SYMBOL_SIZE-1],
    /* input */ ac_int<SYMBOL_BITS, false> left[INPUT_SYMBOL_SIZE-1],
    /* input */ ac_int<SYMBOL_BITS, false> right[INPUT_SYMBOL_SIZE-1],
    /* input */ int num_symbols,
    /* output */ ac_int<SYMBOL_BITS, false> length_histogram[TREE_DEPTH]) {
    assert(num_symbols > 0);
    assert(num_symbols <= INPUT_SYMBOL_SIZE);
    ac_int<TREE_DEPTH_BITS, false> child_depth[INPUT_SYMBOL_SIZE-1];
    ac_int<SYMBOL_BITS, false> internal_length_histogram[TREE_DEPTH];
 init_histogram:
    for(int i = 0; i < TREE_DEPTH; i++) {
        #pragma HLS pipeline II=1
PRAGMA_HLS(HLS unroll factor=copy0)
PRAGMA_HLS(HLS array_partition variable=internal_length_histogram factor=copy0 cyclic)


        internal_length_histogram[i] = 0;
        length_histogram[i] = 0;
    }

    child_depth[num_symbols-2] = 1; // Depth of the root node is 1.

traverse_tree:
    for(int i = num_symbols-3; i >= 0; i--) {
#pragma HLS pipeline II=3
#pragma HLS LOOP_TRIPCOUNT min=1 max=INPUT_SYMBOL_SIZE-2

        ac_int<TREE_DEPTH_BITS, false> length = child_depth[parent[i]] + 1;
        child_depth[i] = length;
        if(left[i] != INTERNAL_NODE || right[i] != INTERNAL_NODE){
            int children;
            if(left[i] != INTERNAL_NODE && right[i] != INTERNAL_NODE) {
                // Both the children of the original node were symbols
                children = 2;
            } else {
                // One child of the original node was a symbol
                children = 1;
            }
            ac_int<SYMBOL_BITS, false> count = internal_length_histogram[length];
            count += children;
            internal_length_histogram[length] = count;
            length_histogram[length] = count;
        }
    }
}
