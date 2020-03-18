#include "huffman.h"
#include "assert.h"
#include <iostream>

//Bit reverse of a Codeword (defined in huffman.h) in place.
void reverse_codeword(Codeword &input) {
	int i;
    Codeword rev = 0;
	reverse: for (i = 0; i < CODEWORD_LENGTH_BITS; i++) {
		rev = (rev << 1) | (input & 1);
		input = input >> 1;
	}
	input = rev;
}

void create_codeword(
  /* input */ CodewordLength symbol_bits[INPUT_SYMBOL_SIZE],
  /* input */ ac_int<SYMBOL_BITS, false> codeword_length_histogram[TREE_DEPTH],
  /* output */ PackedCodewordAndLength encoding[INPUT_SYMBOL_SIZE]
) {
    Codeword first_codeword[MAX_CODEWORD_LENGTH];

    // Computes the initial codeword value for a symbol with bit length i
    first_codeword[0] = 0;
    #pragma hls_pipeline_init_interval 1
    first_codewords: for(int i = 1; i < MAX_CODEWORD_LENGTH; i++) {
        first_codeword[i] = (first_codeword[i-1] + codeword_length_histogram[i-1]) << 1;
//        Codeword c = first_codeword[i];
        //        std::cout << c.to_string(2) << " with length " << i << "\n";
    }
    #pragma hls_pipeline_init_interval 5
    assign_codewords: for (int i = 0; i < INPUT_SYMBOL_SIZE; ++i) {

      CodewordLength length = symbol_bits[i];
      //if symbol has 0 bits, it doesn't need to be encoded
    make_codeword: if(length != 0) {
          //          std::cout << first_codeword[length].to_string(2) << "\n";
          Codeword out_reversed = first_codeword[length];
          reverse_codeword(out_reversed);
          out_reversed = out_reversed >> (MAX_CODEWORD_LENGTH - length);
          // std::cout << out_reversed.to_string(2) << "\n";
          encoding[i] = (out_reversed << CODEWORD_LENGTH_BITS) + length;
          first_codeword[length]++;
      } else {
          encoding[i] = 0;
      }
  }
}
