#include "huffman_params.h"

// input number of symbols
const static int INPUT_SYMBOL_SIZE = 256;

// upper bound on codeword length during tree construction
const static int TREE_DEPTH = 64;

// maximum codeword tree length after rebalancing
const static int MAX_CODEWORD_LENGTH = 27;

// Should be log2(INPUT_SYMBOL_SIZE)
const static int SYMBOL_BITS = 10;

// Should be log2(TREE_DEPTH)
const static int TREE_DEPTH_BITS = 6;

// number of bits needed to record MAX_CODEWORD_LENGTH value
// Should be log2(MAX_CODEWORD_LENGTH)
const static int CODEWORD_LENGTH_BITS = 5;

// A marker for internal nodes
const static unsigned int INTERNAL_NODE = -1;

typedef unsigned int Codeword;
typedef unsigned int PackedCodewordAndLength;
typedef unsigned int CodewordLength;
typedef unsigned int Frequency;

struct Symbol {
	 unsigned int value;
	 unsigned int frequency;
};

void huffman_encoding (
	Symbol in[INPUT_SYMBOL_SIZE],
	PackedCodewordAndLength encoding[INPUT_SYMBOL_SIZE],
	int *num_nonzero_symbols
);






void filter(Symbol in[INPUT_SYMBOL_SIZE],
            Symbol out[INPUT_SYMBOL_SIZE],
            int *num_symbols);
void sort(Symbol in[INPUT_SYMBOL_SIZE],
          int num_symbols,
          Symbol out[INPUT_SYMBOL_SIZE]);
void create_tree (
    Symbol in[INPUT_SYMBOL_SIZE],
    int num_symbols,
    unsigned int parent[INPUT_SYMBOL_SIZE-1],
    unsigned int left[INPUT_SYMBOL_SIZE-1],
    unsigned int right[INPUT_SYMBOL_SIZE-1]);

void compute_bit_length (
  unsigned int parent[INPUT_SYMBOL_SIZE-1],
  unsigned int left[INPUT_SYMBOL_SIZE-1],
  unsigned int right[INPUT_SYMBOL_SIZE-1],
  int num_symbols,
  unsigned int bit_length[TREE_DEPTH]);

void truncate_tree(
	unsigned int input_bit_length[TREE_DEPTH],
	unsigned int truncated_bit_length1[TREE_DEPTH],
	unsigned int truncated_bit_length2[TREE_DEPTH]);

void canonize_tree(
	Symbol sorted[INPUT_SYMBOL_SIZE],
	unsigned int num_symbols,
	unsigned int bit_length[TREE_DEPTH],
	CodewordLength symbol_bits[INPUT_SYMBOL_SIZE]);

void create_codeword(
  CodewordLength symbol_bits[INPUT_SYMBOL_SIZE],
  unsigned int bit_length[TREE_DEPTH],
  PackedCodewordAndLength encoding[INPUT_SYMBOL_SIZE]);


static unsigned int bit_reverse32(unsigned int input) {
	int i, rev = 0;
	for (i = 0; i < 32; i++) {
		rev = (rev << 1) | (input & 1);
		input = input >> 1;
	}
	return rev;
}
