create_clock -period 0.02 -name clk

set_input_delay  0.0015 -clock clk pi1 
set_input_delay  0.002 -clock clk pi2 

set_max_delay  0.015 -to po0 
