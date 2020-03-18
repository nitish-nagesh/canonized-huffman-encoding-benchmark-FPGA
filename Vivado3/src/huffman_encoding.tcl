open_project huffman_encoding.proj -reset
add_files {huffman_canonize_tree.cpp huffman_create_tree.cpp huffman_filter.cpp huffman_compute_bit_length.cpp huffman_encoding.cpp huffman_sort.cpp huffman_create_codeword.cpp huffman_truncate_tree.cpp}

add_files -tb {huffman_encoding_test.cpp}
add_files -tb {huffman.random256.txt huffman.random256.golden}
set_top huffman_encoding
#set_top create_tree
open_solution solution -reset
set_part {xcvu9p-flga2104-1-e} -tool vivado
create_clock -period 10
#csim_design -compiler clang
#csynth_design
