create_clock -period 0.03 -name clk

set_input_delay  0.00026 -clock clk B 
set_input_delay  0.0012 -clock clk A 
set_input_delay  0.00015 -clock clk D 
set_input_delay  0.0005 -clock clk C 
