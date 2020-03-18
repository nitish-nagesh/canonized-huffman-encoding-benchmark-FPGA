open_project huffman_encoding
set_top huffman_encoding
add_files huffman.h
add_files {huffman_canonize_tree.cpp huffman_create_tree.cpp huffman_filter.cpp huffman_compute_bit_length.cpp huffman_encoding.cpp huffman_sort.cpp huffman_create_codeword.cpp huffman_truncate_tree.cpp}
add_files huffman_params.h
add_files -tb huffman_encoding_test.cpp
add_files -tb {huffman.random256.txt huffman.random256.golden}

# Solution
open_solution -reset "solutionx"
set_part {xc7z020clg400-1} -tool vivado
create_clock -period 10
csim_design
csynth_design

exit