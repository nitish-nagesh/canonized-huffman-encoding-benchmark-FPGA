#include "huffman.h"
#include "assert.h"
void compute_bit_length (
    /* input */ int parent[INPUT_SYMBOL_SIZE-1],
    /* input */ int left[INPUT_SYMBOL_SIZE-1],
    /* input */ int right[INPUT_SYMBOL_SIZE-1],
    /* input */ int num_symbols,
    /* output */ int length_histogram[TREE_DEPTH]) {
    assert(num_symbols > 0);
    assert(num_symbols <= INPUT_SYMBOL_SIZE);
    int child_depth[INPUT_SYMBOL_SIZE-1];
    int internal_length_histogram[TREE_DEPTH];

    #pragma hls_pipeline_init_interval 1
    init_histogram: for(int i = 0; i < TREE_DEPTH; i++) {
        internal_length_histogram[i] = 0;
        length_histogram[i] = 0;
    }

    child_depth[num_symbols-2] = 1; // Depth of the root node is 1.

    #pragma hls_pipeline_init_interval 3
    traverse_tree: for(int i = num_symbols-3; i >= 0; i--) {

        int length = child_depth[parent[i]] + 1;
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
            int count = internal_length_histogram[length];
            count += children;
            internal_length_histogram[length] = count;
            length_histogram[length] = count;
        }
    }
}
