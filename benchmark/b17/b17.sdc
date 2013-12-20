create_clock -period 0.03 -name clk

set_input_delay  0.00018 -clock clk pi36 
set_input_delay  0.00034 -clock clk pi29 

set_max_delay  0.022 -to po14 
set_max_delay  0.026 -to po22 
