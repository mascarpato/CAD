create_clock -period 0.03 -name clk

set_input_delay  0.0014 -clock clk pi36 
set_input_delay  0.01 -clock clk pi05 
set_input_delay  0.008 -clock clk pi02 
set_input_delay  0.0075 -clock clk pi33 

set_max_delay  0.028 -to po14 
set_max_delay  0.024 -to po22 
set_max_delay  0.02 -to po17 
set_max_delay  0.027 -to po25 