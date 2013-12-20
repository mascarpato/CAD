create_clock -period 0.02 -name clk

set_input_delay  0.0012 -clock clk pi4 
set_input_delay  0.00048 -clock clk pi0 

set_max_delay  0.015 -to po1 
