Vivado to Catapult Translations
===============================

NOTE: For Catapult move pragmas above loop (and above loop name)

Pipeline
--------
#pragma HLS PIPELINE II=5
#pragma hls_pipeline_init_interval 1

Unroll
------
#pragma HLS unroll factor=2
#pragma hls_unroll yes (factor?)

Array Partition
---------------
#pragma HLS array_partition variable=arr factor=2 cyclic

Loop Tripcount
--------------
#pragma HLS loop_tripcount min=MIN avg=AVG max= MAX


Inline
------
#pragma HLS inline off


Dataflow
--------
#pragma HLS dataflow

