create_clock -period 0.03 -name clk

set_input_delay  0.000146 -clock clk pi23 
set_input_delay  0.002 -clock clk pi02 
set_input_delay  0.000058 -clock clk pi14 
set_input_delay  0.0016 -clock clk pi22 
set_input_delay  0.00042 -clock clk pi08 
set_input_delay  0.00067 -clock clk pi10 

set_max_delay  0.028 -to po04 