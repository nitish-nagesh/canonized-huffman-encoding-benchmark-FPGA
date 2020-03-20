#include "huffman.h"
#include "assert.h"
void canonize_tree(
    /* input */ Symbol sorted[INPUT_SYMBOL_SIZE],
    /* input */ ac_int<SYMBOL_BITS, false> num_symbols,
    /* input */ ac_int<SYMBOL_BITS, false> codeword_length_histogram[TREE_DEPTH],
    /* output */ CodewordLength symbol_bits[INPUT_SYMBOL_SIZE] ) {
    // assert(num_symbols <= INPUT_SYMBOL_SIZE);

    init_bits: for(int i = 0; i < INPUT_SYMBOL_SIZE; i++) {
        symbol_bits[i] = 0;
    }
    
    ac_int<SYMBOL_BITS, false> length = TREE_DEPTH;
    ac_int<SYMBOL_BITS, false> count = 0;

    // Iterate across the symbols from lowest frequency to highest
    // Assign them largest bit length to smallest
    process_symbols: for(int k = 0; k < num_symbols; k++) {
        if (count == 0) {
            //find the next non-zero bit length
            for(int a = 0; a < TREE_DEPTH; a++) {
                length--;
                // n is the number of symbols with encoded length i
                count = codeword_length_histogram[length];
                if(count != 0) { break; }
            }
        }
        symbol_bits[sorted[k].value] = length; //assign symbol k to have length bits
        count--; //keep assigning i bits until we have counted off n symbols
    }
}
