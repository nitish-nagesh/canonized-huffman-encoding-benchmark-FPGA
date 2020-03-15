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
<!-- DONE USING DIRECTIVES.TCL -->

Array Partition
---------------
#pragma HLS array_partition variable=arr factor=2 cyclic
<!-- DONE USING DIRECTIVES.TCL -->

Loop Tripcount
--------------
#pragma HLS loop_tripcount min=MIN avg=AVG max= MAX
<!-- DONE IN DIRECTIVES.TCL -->

Inline
------
#pragma HLS inline off


Dataflow
--------
#pragma HLS dataflow

