create_clock -period 0.02 -name clk

set_input_delay  0.0000067 -clock clk pi33 
set_input_delay  0.00037 -clock clk pi26 

set_max_delay  0.018 -to po20 
set_max_delay  0.017 -to po17 
