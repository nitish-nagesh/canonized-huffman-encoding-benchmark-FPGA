#include "huffman.h"
#include "huffman_params.h"
// Postcondition: out[x].frequency > 0
void filter(
            /* input  */ Symbol in[INPUT_SYMBOL_SIZE],
            /* output */ Symbol out[INPUT_SYMBOL_SIZE],
            /* output */ int *n) {
    //#pragma HLS INLINE off
    int j = 0;
    #pragma hls_pipeline_init_interval 1
    filter_main: for(int i = 0; i < INPUT_SYMBOL_SIZE; i++) {
        if(in[i].frequency != 0) {
            out[j].frequency = in[i].frequency;
            out[j].value = in[i].value;
            j++;
        }
    }
    *n = j;
}
