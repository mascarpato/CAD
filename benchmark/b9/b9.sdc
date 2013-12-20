create_clock -period 0.03 -name clk

set_input_delay  0 -clock clk pi4 
set_input_delay  0 -clock clk pi0 
set_input_delay  0 -clock clk pi3 
set_input_delay  0 -clock clk pi1 
set_input_delay  0 -clock clk pi2 

set_max_delay  0.03 -to po1 
set_max_delay  0.03 -to po0 
