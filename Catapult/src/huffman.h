#include "huffman_params.h"
#include <ac_math.h>

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
const static int INTERNAL_NODE = -1;

typedef int Codeword;
typedef int PackedCodewordAndLength;
typedef int CodewordLength;
typedef int Frequency;

struct Symbol {
	 int value;
	 int frequency;
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
    int parent[INPUT_SYMBOL_SIZE-1],
    int left[INPUT_SYMBOL_SIZE-1],
    int right[INPUT_SYMBOL_SIZE-1]);

void compute_bit_length (
  int parent[INPUT_SYMBOL_SIZE-1],
  int left[INPUT_SYMBOL_SIZE-1],
  int right[INPUT_SYMBOL_SIZE-1],
  int num_symbols,
  int bit_length[TREE_DEPTH]);

void truncate_tree(
	int input_bit_length[TREE_DEPTH],
	int truncated_bit_length1[TREE_DEPTH],
	int truncated_bit_length2[TREE_DEPTH]);

void canonize_tree(
	Symbol sorted[INPUT_SYMBOL_SIZE],
	int num_symbols,
	int bit_length[TREE_DEPTH],
	CodewordLength symbol_bits[INPUT_SYMBOL_SIZE]);

void create_codeword(
  CodewordLength symbol_bits[INPUT_SYMBOL_SIZE],
  int bit_length[TREE_DEPTH],
  PackedCodewordAndLength encoding[INPUT_SYMBOL_SIZE]);


static int bit_reverse32(int input) {
	int i, rev = 0;
	for (i = 0; i < 32; i++) {
		rev = (rev << 1) | (input & 1);
		input = input >> 1;
	}
	return rev;
}
