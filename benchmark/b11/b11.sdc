create_clock -period 0.04 -name clk

set_input_delay  0.002 -clock clk B 
set_input_delay  0.0015 -clock clk A 
