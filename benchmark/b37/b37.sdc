create_clock -period 0.03 -name clk

set_input_delay  0.0029 -clock clk pi06 
set_input_delay  0.005639 -clock clk pi05 

set_max_delay  0.02 -to po18 